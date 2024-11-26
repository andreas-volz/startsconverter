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
  std::vector<std::string> wpeTilesetNames = mPaletteManager.getTilesetNames();

  for(string tileset_str : wpeTilesetNames)
  {
    tileset::TilesetHub tilesethub(mHurricane, tileset_str);

    tilesethub.generateVF4Json(tilesetStorage);
    tilesethub.generateCV5Json(tilesetStorage);
  }
}
