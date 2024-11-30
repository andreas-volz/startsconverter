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
#include "PaletteConverter.h"


class GraphicsConverter : public Converter
{
public:
  GraphicsConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub, PaletteConverter &palette_converter);
  virtual ~GraphicsConverter();

  void convert(Storage graphics);

private:
  dat::DataHub &mDatahub;
  PaletteConverter &mPaletteConverter;
};

#endif /* GRAPHICSCONVERTER_H */
