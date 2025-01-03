/*
 * MiniTile.cpp
 *
 *      Author: Andreas Volz
 */

#include "MiniTile.h"

using namespace std;

namespace tileset
{

MiniTile::MiniTile(TilesetHub &tilesethub, size_t position) :
    mPaletteImage(make_shared<PaletteImage>(Vector2i(8, 8)))
{
  tileset_vr4_t::pixel_type_t* pixel_ref = tilesethub.vr4->array()->at(position);
  const string &color = pixel_ref->color_index();

  /* copy the color index positions into the palette image to replace them later in the PNGExporter with the
   * color palette RGB information
   */
  unsigned int i = 0;
  for (auto c : color)
  {
    Vector2i pos = mPaletteImage->indexToPosition(i);
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
