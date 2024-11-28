/*
 * MegaTile.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "MegaTile.h"
#include "Logger.h"
#include "tileset/MiniTile.h"

// system
#include <string>
#include <iostream>
#include <memory>

using namespace std;

namespace tileset
{

static Logger logger = Logger("startool.tileset.MegaTile");

MegaTile::MegaTile(TilesetHub &tilesethub, size_t position) :
  mPaletteImage(make_shared<TiledPaletteImage>(Size(4, 4), Size(8, 8)))
{
  tileset_vx4_t::megatile_type_t *megatile = tilesethub.vx4->array()->at(position);

  std::vector<tileset_vx4_t::graphic_ref_type_t *> *vx4_array_graphic_ref = megatile->graphic_ref();

  unsigned int n = 0;
  for (auto vx4_graphic_ref : *vx4_array_graphic_ref)
  {
    uint64_t vr4_ref = vx4_graphic_ref->vr4_ref();
    bool horizontal_flip = vx4_graphic_ref->horizontal_flip();

    MiniTile minitile(tilesethub, vr4_ref);
    shared_ptr<PaletteImage> paletteImage = minitile.getPaletteImage();

    mPaletteImage->copyTile(*paletteImage, n, horizontal_flip);

    n++;
  }
}

MegaTile::~MegaTile()
{

}

std::shared_ptr<PaletteImage> MegaTile::getPaletteImage()
{
  return mPaletteImage;
}
} /* namespace tileset */
