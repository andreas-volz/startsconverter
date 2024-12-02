/*
 * CursorConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef CURSORCONVERTER_H
#define CURSORCONVERTER_H

/* project */
#include "Converter.h"
#include "PaletteConverter.h"

class CursorConverter : public Converter
{
public:
  CursorConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter);
  virtual ~CursorConverter();

  void convert(Storage cursorStorage);

private:
  PaletteConverter &mPaletteConverter;
};

#endif /* CURSORCONVERTER_H */
