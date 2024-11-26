/*
 * GraphicsConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef GRAPHICSCONVERTER_H
#define GRAPHICSCONVERTER_H

// project
#include "Converter.h"
#include "dat/DataHub.h"
#include "PaletteManager.h"


class GraphicsConverter : public Converter
{
public:
  GraphicsConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub, PaletteManager &palette_manager);
  virtual ~GraphicsConverter();

  void convert(Storage graphics);

private:
  dat::DataHub &mDatahub;
  PaletteManager &mPaletteManager;
};

#endif /* GRAPHICSCONVERTER_H */
