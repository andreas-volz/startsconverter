/*
 * TilesetConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef TILESETCONVERTER_H
#define TILESETCONVERTER_H

/* project */
#include "Converter.h"
#include "PaletteManager.h"
#include "tileset/TilesetHub.h"

/* system */

class TilesetConverter : public Converter
{
public:
  TilesetConverter(std::shared_ptr<Hurricane> hurricane, PaletteManager &palette_manager);
  virtual ~TilesetConverter();

  void convert(Storage tilesetStorage);

private:
  PaletteManager &mPaletteManager;
};

#endif /* TILESETCONVERTER_H */
