/*
 * MegaTile.h
 *
 *      Author: Andreas Volz
 */

#ifndef MEGATILE_H
#define MEGATILE_H

/* project */
#include "TilesetHub.h"
#include "TiledPaletteImage.h"

namespace tileset
{

/**
 * MiniTiles are arranged in a MegaTile as follows:
 * 0 |  1 | 2 | 3
 * 4 |  5 | 6 | 7
 * 8 |  9 |10 |11
 * 12| 13 |14 |15
 *
 * It doesn't store the MiniTile itself, but only uses them as creator for the image data of the MegaTile
 *
 */
class MegaTile
{
public:
  /**
   * @param tilesethub All the data files for a specific read Tileset
   * @param  position The VX4 array position with the image data that is used to construct this MegaTile
   */
  MegaTile(TilesetHub &tilesethub, size_t position);
  virtual ~MegaTile();

  std::shared_ptr<PaletteImage> getPaletteImage();

private:
  std::shared_ptr<TiledPaletteImage> mPaletteImage;
};

} /* namespace tileset */

#endif /* MEGATILE_H */
