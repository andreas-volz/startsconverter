/*
 * TiledPaletteImage.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "TiledPaletteImage.h"
#include "Logger.h"

/* system */
#include <string>

using namespace std;

namespace tileset
{

static Logger logger = Logger("startool.tileset.TiledPaletteImage");

TiledPaletteImage::TiledPaletteImage(const Vector2i &tileSize, const Vector2i &subtileSize) :
    PaletteImage(tileSize * subtileSize),
    mTileSize(tileSize),
    mSubtileSize(subtileSize)
{

}

TiledPaletteImage::~TiledPaletteImage()
{

}

void TiledPaletteImage::copyTile(const PaletteImage &palette_image, size_t index, bool horizontal_flip)
{
  int y = 0;
  int x = 0;

  // if index is out of data size that return Pos(0, 0) as fail safe
  if((int)index < (getSize().x * getSize().y) /
      (palette_image.getSize().x * palette_image.getSize().y))
  {
    y = index / (getSize().x / palette_image.getSize().x);
    x = index % (getSize().x / palette_image.getSize().x);
  }

  Vector2i rel_pos(x,y);
  copyTile(palette_image, rel_pos, horizontal_flip);
}

void TiledPaletteImage::copyTile(const PaletteImage &palette_image, const Vector2i &pos, bool horizontal_flip)
{
  if(pos.x < mTileSize.x || pos.y < mTileSize.x)
  {

    for(int x = 0; x < palette_image.getSize().x; x++)
    {
      for(int y = 0; y < palette_image.getSize().y; y++)
      {
        unsigned char pixel = palette_image.at(Vector2i(x, y));

        int x_flip = x;
        if(horizontal_flip)
        {
          x_flip = palette_image.getSize().x - 1 - x;
        }

        at(calcAbsolutePos(pos, Vector2i(x_flip, y))) = pixel;
      }
    }
  }
  else
  {
    LOG4CXX_WARN(logger, "copyTile() out of range! Pos(" + to_string(pos.x) + "/" + to_string(pos.y) + ")");
  }
}

const Vector2i TiledPaletteImage::calcAbsolutePos(const Vector2i &tile_pos, const Vector2i &relative_pos)
{
  return Vector2i(tile_pos.x * mSubtileSize.x + relative_pos.x, tile_pos.y * mSubtileSize.y + relative_pos.y);
}

} /* namespace tileset */
