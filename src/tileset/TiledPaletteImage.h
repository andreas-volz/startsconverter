/*
 * TiledPaletteImage.h
 *
 *      Author: Andreas
 */

#ifndef TILEDPALETTEIMAGE_H_
#define TILEDPALETTEIMAGE_H_

// project
#include "PaletteImage.h"

namespace tileset
{

/**
 * A TiledPaletteImage is a special form of a PaletteImage. As child it shares all details,
 * but with the extension that complete other PaletteImages could be added at once to a specific
 * Tile. The pixels are just copied to the target after inserting a tile. So afterwards the
 * TiledPaletteImage could just be used as normal PaletteImage or put into another TiledPaletteImage.
 */
class TiledPaletteImage: public PaletteImage
{
public:
  TiledPaletteImage(const Vector2i &tileSize, const Vector2i &subtileSize);

  virtual ~TiledPaletteImage();

  void copyTile(const PaletteImage &palette_image, size_t index, bool horizontal_flip = false);

  void copyTile(const PaletteImage &palette_image, const Vector2i &pos, bool horizontal_flip = false);

private:
  const Vector2i calcAbsolutePos(const Vector2i &tile_pos, const Vector2i &relative_pos);

  Vector2i mTileSize;
  Vector2i mSubtileSize;
};

} /* namespace tileset */

#endif /* TILEDPALETTEIMAGE_H_ */
