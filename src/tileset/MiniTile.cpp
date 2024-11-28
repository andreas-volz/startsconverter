/*
 * MiniTile.cpp
 *
 *      Author: Andreas Volz
 */

#include "MiniTile.h"

using namespace std;

namespace tileset
{

MiniTile::MiniTile(TilesetHub &tilesethub, size_t element) :
    mPaletteImage(make_shared<PaletteImage>(Size(8, 8)))
{
  tileset_vr4_t::pixel_type_t* pixel_ref = tilesethub.vr4->elements()->at(element);
  const string &color = pixel_ref->minitile(); // TODO: maybe change the kaitai API name to color_index() or similar

  /* copy the color index positions into the palette image to replace them later in the PNGExporter with the
   * color palette RGB information
   */
  unsigned int i = 0;
  for (auto c : color)
  {
    Pos pos = mPaletteImage->indexToPosition(i);
    mPaletteImage->at(pos) = c;

    i++;
  }
}

MiniTile::~MiniTile()
{
}

std::shared_ptr<PaletteImage> MiniTile::getPaletteImage()
{
  return mPaletteImage;
}

} /* namespace tileset */
