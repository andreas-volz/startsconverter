/*
 * Pcx.cpp
 *
 *      Author: Andreas Volz
 */

// Local
#include <PngExporter.h>
#include "endian.h"
#include "Pcx.h"
#include "Storm.h"
#include "Hurricane.h"
#include "Logger.h"
#include "FileUtil.h"

// System
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

static Logger logger = Logger("startool.Pcx");

Pcx::Pcx(std::shared_ptr<Hurricane> hurricane) :
  Converter(hurricane), rawImage(0)
{
}

Pcx::Pcx(std::shared_ptr<Hurricane> hurricane, const std::string &arcfile) :
  Converter(hurricane), rawImage(0)
{
  load(arcfile);
}

Pcx::~Pcx()
{
  free(rawImage);
}

bool Pcx::load(const std::string &arcfile)
{
  bool result = true;

  mRawData = mHurricane->extractDataChunk(arcfile);
  if (mRawData)
  {
    free(rawImage);
    extractHeader();
    extractImage();

    LOG4CXX_TRACE(logger, "load(): " + arcfile);
  }
  else
  {
    result = false;
  }

  return result;
}

bool Pcx::savePNG(Storage storage)
{
  bool result = true;

  if (mRawData)
  {
    PngExporter::saveRGB(storage.getFullPath(), *mPaletteImage, *mPalette, 0);
  }
  else
  {
    result = false;
  }

  return result;
}

bool Pcx::saveRGBMapJson(Storage storage)
{
  bool result = true;

  json j_color_list;

  if(mPaletteImage->getSize().y != 1)
  {
    // those special color images in starcraft are only one pixel height
    return false;
  }

  CheckPath(storage);

  for(int i = 0; i < mPaletteImage->getSize().x; i++)
  {
    //json j_rgb;
    unsigned char pixel = mPaletteImage->at(i);
    Color &color = mPalette->at(pixel);
    j_color_list.push_back(color.getRed());
    j_color_list.push_back(color.getGreen());
    j_color_list.push_back(color.getBlue());
    //j_color_list.push_back(j_rgb);
  }

  saveJson(j_color_list, storage, true);

  return result;
}

std::shared_ptr<Palette> Pcx::getPalette()
{
  return make_shared<Palette>(*mPalette);
}

Vector2i Pcx::getSize()
{
  Vector2i imageSize;

  if(mPaletteImage)
  {
    imageSize = mPaletteImage->getSize();
  }

  return imageSize;
}

std::shared_ptr<Palette> Pcx::mapIndexPalette(int length, int start, int index)
{
  std::shared_ptr<Palette> palette(mPalette);

  if (mPaletteImage && palette)
  {
    for (int i = 0; i < length; i++)
    {
      int rel_index = i + (index * length);
      int start_pal_dest = start + i;

      unsigned char color_index = mPaletteImage->at(rel_index);

      Color &image_color = mPalette->at(color_index);
      palette->at(start_pal_dest) = image_color;
    }
  }

  return palette;
}

std::shared_ptr<Palette2D> Pcx::map2DPalette()
{
  std::shared_ptr<Palette2D> palette2D;

  if (mPaletteImage && mPalette)
  {
    // check magic size of the special 2D palette format
    if (mPaletteImage->getSize().x == 256)
    {
      palette2D = make_shared<Palette2D>(mPaletteImage->getSize().y);

      for (int x = 0; x < mPaletteImage->getSize().x-1; x++)
      {
        for (int y = 0; y < mPaletteImage->getSize().y-1; y++)
        {
          // FIXME: there was a bug in getPalettePixel() that y was decremented 1. After fixing it the output was ugly
          // now I substract here y-1 before and now alpha graphics (e.g. fire) look fine. I've to check the algorithm later again
          int y_mod = (y > 0) ? (y-1) : 0;

          unsigned char color_index = mPaletteImage->at(Vector2i(x, y_mod));

          if(color_index != 255)
          {
            Color image_color = mPalette->at(color_index);

            palette2D->at(x, y) = image_color;
          }
          else
          {
            Color trans_color(0, 0, 0);
            palette2D->at(x, y) = trans_color;
          }
        }
      }
    }
  }

  return palette2D;
}

void Pcx::extractHeader()
{
  if (mRawData)
  {
    struct PCXheader pcxh;

    memcpy(&pcxh, mRawData->getDataPointer(), sizeof(struct PCXheader));
    pcxh.Xmin = ConvertLE16(pcxh.Xmin);
    pcxh.Ymin = ConvertLE16(pcxh.Ymin);
    pcxh.Xmax = ConvertLE16(pcxh.Xmax);
    pcxh.Ymax = ConvertLE16(pcxh.Ymax);
    pcxh.BytesPerLine = ConvertLE16(pcxh.BytesPerLine);

    int width = pcxh.Xmax - pcxh.Xmin + 1;
    int height = pcxh.Ymax - pcxh.Ymin + 1;

    mPaletteImage = make_shared<PaletteImage>(Vector2i(width, height));
  }
}

void Pcx::extractImage()
{
  int y;
  int count;
  unsigned char *dest = NULL;
  unsigned char ch = 0;
  unsigned char *imageParserPos = nullptr;
  size_t pos = 0;

  if (mRawData)
  {
    rawImage = (unsigned char *) malloc(mPaletteImage->getSize().x * mPaletteImage->getSize().y);
    imageParserPos = mRawData->getDataPointer() + sizeof(struct PCXheader);

    for (y = 0; y < mPaletteImage->getSize().y; ++y)
    {
      count = 0;
      dest = rawImage + y * mPaletteImage->getSize().x;
      for (int i = 0; i < mPaletteImage->getSize().x; ++i)
      {
        if (!count)
        {
          ch = *imageParserPos++;
          if ((ch & 0xc0) == 0xc0)
          {
            count = ch & 0x3f;
            ch = *imageParserPos++;
          }
          else
          {
            count = 1;
          }
        }
        dest[i] = ch;
        mPaletteImage->at(pos) = ch;
        pos++;
        --count;
      }
    }

    // extract palette =>

    mPalette = make_shared<Palette>();

    do
    {
      ch = *imageParserPos++;
    }
    while (ch != 0x0c);   // search the 'magic ID' that shows the start of RGB information next

    // copy RGB information to destination
    for (int i = 0; i < RGB_SIZE; i += 3)
    {
      unsigned char red = *imageParserPos++;
      unsigned char green = *imageParserPos++;
      unsigned char blue = *imageParserPos++;

      Color rgb(red, green, blue);

      mPalette->at(i/3) = rgb;
    }

  }
}

void Pcx::saveJson(json &j, const std::string &file, bool pretty)
{
  std::ofstream filestream(file);

  if(pretty)
  {
    filestream << std::setw(4) << j;
  }
  else
  {
    filestream << j;
  }
}
