/*
 * TilesetConverter.cpp
 *
 *      Author: Andreas Volz
 */

#include "TilesetConverter.h"

using namespace std;

TilesetConverter::TilesetConverter(std::shared_ptr<Hurricane> hurricane, PaletteManager &palette_manager) :
  Converter(hurricane),
  mPaletteManager(palette_manager)
{
}

TilesetConverter::~TilesetConverter()
{
}

void TilesetConverter::convert(Storage tilesetStorage)
{
  /* TODO: If this idea doesn't work for Broodwar then get all valid names from all .chk files before... */
  std::vector<std::string> wpeTilesetNames = mPaletteManager.getTilesetNames();

  for(string tileset_str : wpeTilesetNames)
  {
    tileset::TilesetHub tilesethub(mHurricane, tileset_str);

    tilesethub.generateCV5Json(tilesetStorage);
    tilesethub.generateVF4Json(tilesetStorage);
    tilesethub.generateVX4Json(tilesetStorage);
    tilesethub.convertTiledFormat(mPaletteManager.getPalette(tileset_str), tilesetStorage);

  }
}
