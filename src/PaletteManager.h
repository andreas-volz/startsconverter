/*
 * PaletteManager.h
 *
 *      Author: Andreas Volz
 */

#ifndef PALETTEMANAGER_H
#define PALETTEMANAGER_H

/* project */
#include "Converter.h"
#include "AbstractPalette.h"
#include "Storage.h"

/* system */
#include <map>

class PaletteManager : public Converter
{
public:
  PaletteManager(std::shared_ptr<Hurricane> hurricane);
  virtual ~PaletteManager();

  void convert(Storage palStorage);

  std::shared_ptr<AbstractPalette> getPalette(const std::string palette_name);

private:
  std::map<std::string, std::shared_ptr<AbstractPalette>> mPaletteMap;
};

#endif /* PALETTEMANAGER_H */
