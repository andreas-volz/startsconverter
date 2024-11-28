/*
 * MiniTile.h
 *
 *      Author: Andreas Volz
 */

#ifndef MINITILE_H
#define MINITILE_H

#include "TilesetHub.h"
#include "TiledPaletteImage.h"

namespace tileset
{

class MiniTile
{
public:
  MiniTile(TilesetHub &tilesethub, size_t element);
  virtual ~MiniTile();

  std::shared_ptr<PaletteImage> getPaletteImage();

private:
  std::shared_ptr<PaletteImage> mPaletteImage;
};

} /* namespace tileset */

#endif /* MINITILE_H */
