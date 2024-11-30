/*
 * TilesetConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef TILESETCONVERTER_H
#define TILESETCONVERTER_H

/* project */
#include "Converter.h"
#include "tileset/TilesetHub.h"

/* system */
#include <map>
#include "PaletteConverter.h"


class TilesetConverter : public Converter
{
public:
  TilesetConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter);
  virtual ~TilesetConverter();

  void convert(Storage tilesetStorage);

  std::shared_ptr<tileset::TilesetHub> getTilesetHub(std::string tileset);

private:
  PaletteConverter &mPaletteConverter;
  std::map<std::string, std::shared_ptr<tileset::TilesetHub>> mTilesetHubMap;
};

#endif /* TILESETCONVERTER_H */
