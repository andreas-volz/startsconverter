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
  /**
   * A MiniTile has 8x8 pixels. It stores information to a indexed color palette, but not the RGB color information itself.
   * Those are added later in the PNGExporter.
   *
   * @param tilesethub All the data files for a specific read Tileset
   * @param  position The VR4 array position with the image data that is used to construct this MiniTile
   */
  MiniTile(TilesetHub &tilesethub, size_t position);
  virtual ~MiniTile();

  std::shared_ptr<PaletteImage> getPaletteImage();

private:
  std::shared_ptr<PaletteImage> mPaletteImage;
};

} /* namespace tileset */

#endif /* MINITILE_H */
