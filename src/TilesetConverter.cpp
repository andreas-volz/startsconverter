/*
 * TilesetConverter.cpp
 *
 *      Author: Andreas Volz
 */

#include "TilesetConverter.h"

using namespace std;

TilesetConverter::TilesetConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter) :
  Converter(hurricane),
  mPaletteConverter(paletteConverter)
{
}

TilesetConverter::~TilesetConverter()
{
}

void TilesetConverter::convert(Storage tilesetStorage)
{
  /* TODO: If this idea doesn't work for Broodwar then get all valid names from all .chk files before... */
  std::vector<std::string> wpeTilesetNames = mPaletteConverter.getTilesetNames();

  for(string tileset_str : wpeTilesetNames)
  {
    std::shared_ptr<tileset::TilesetHub> tilesethub = make_shared<tileset::TilesetHub>(mHurricane, tileset_str);
    mTilesetHubMap[tileset_str] = tilesethub;

    tilesethub->generateCV5Json(tilesetStorage);
    tilesethub->generateVF4Json(tilesetStorage);
    tilesethub->generateVX4Json(tilesetStorage);
    tilesethub->convertTiledFormat(mPaletteConverter.getPalette(tileset_str), tilesetStorage);

  }
}

std::shared_ptr<tileset::TilesetHub> TilesetConverter::getTilesetHub(std::string tileset)
{
  return mTilesetHubMap[tileset];
}
