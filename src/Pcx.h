/*
 * Pcx.h
 *
 *      Author: Andreas Volz
 */

#ifndef PCX_H
#define PCX_H

/* project */
#include "Converter.h"
#include "Palette.h"
#include "Storage.h"
#include "PaletteImage.h"
#include "Palette2D.h"

/* system */
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Pcx : public Converter
{
public:
  Pcx(std::shared_ptr<Hurricane> hurricane);
  Pcx(std::shared_ptr<Hurricane> hurricane, const std::string &arcfile);
  virtual ~Pcx();

  bool load(const std::string &arcfile);

  /**
   *  Convert a PCX graphic to PNG format.
   *  The PCX files is always saved with the original unmapped palette as mapping is
   *  only relevant for GRP colors.
   *
   *  @param storage where to save the PNG image
   */
  bool savePNG(Storage storage);


  /**
   * Definition on an RGBMap is a PCX file which is one pixel height and has information about palette remapping colors.
   * One possibility is to map them in the offline converter tools and convert mapped PNG files. But this feature allows
   * to export them in a JSON file that modern engines could use a shader to remap colors on-the-fly.
   */
  bool saveRGBMapJson(Storage storage);

  /**
   *
   * @return get a copy of the unmodified original pcx palette information
   */
  std::shared_ptr<Palette> getPalette();

  /**
   * Map palette colors from one place in the image to a position in the color palette
   *
   * @param length the length of one color index
   * @param start where to place the copy in the palette position
   * @param index the color index
   *
   * @return a new created Palette with the applied mapping
   */
  std::shared_ptr<Palette> mapIndexPalette(int length, int start, int index);

  /**
   * * @return a new created Palette2D with the applied mapping
   */
  std::shared_ptr<Palette2D> map2DPalette();

  Size getSize();

private:
  struct PCXheader
  {
    unsigned char Manufacturer;
    unsigned char Version;
    unsigned char Encoding;
    unsigned char BitsPerPixel;
    short Xmin, Ymin, Xmax, Ymax;
    short HDpi, VDpi;
    unsigned char Colormap[48];
    unsigned char Reserved;
    unsigned char NPlanes;
    short BytesPerLine;
    short PaletteInfo;
    short HscreenSize;
    short VscreenSize;
    unsigned char Filler[54];
  };

  void extractHeader();

  void saveJson(nlohmann::json &j, const std::string &file, bool pretty);

  /**
   *  Convert 8 bit pcx file to raw image
   */
  void extractImage();

  std::shared_ptr<Palette> mPalette;
  std::shared_ptr<DataChunk> mRawData;
  std::shared_ptr<PaletteImage> mPaletteImage;
  unsigned char *rawImage;
  const int RGB_BYTE_SIZE = 3;
  const int RGB_SIZE = 256 * RGB_BYTE_SIZE;
};

#endif /* PCX_H */
