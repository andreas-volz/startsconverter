/*
 * WidgetsConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef WIDGETSCONVERTER_H
#define WIDGETSCONVERTER_H

/* project */
#include "Converter.h"
#include "Storage.h"
#include "PaletteConverter.h"

class WidgetsConverter : public Converter
{
public:
  WidgetsConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter);
  virtual ~WidgetsConverter();

  void convert(Storage widgetStorage);

private:
  PaletteConverter &mPaletteConverter;
};

#endif /* WIDGETSCONVERTER_H */
