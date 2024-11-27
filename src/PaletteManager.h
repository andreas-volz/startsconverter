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
#include "Logger.h"

/* system */
#include <map>

class PaletteManager : public Converter
{
public:
  PaletteManager(std::shared_ptr<Hurricane> hurricane);
  virtual ~PaletteManager();

  void convert(Storage palStorage);

  std::shared_ptr<AbstractPalette> getPalette(const std::string palette_name);

  /**
   * TODO: this needs to be checked for Broodwar data generation.
   *
   * @return the list of tileset in the data package (and the ones yet saved as .pal)
   */
  std::vector<std::string> getTilesetNames();

private:
  std::map<std::string, std::shared_ptr<AbstractPalette>> mPCXPaletteMap;
  std::map<std::string, std::shared_ptr<AbstractPalette>> mPCX2DPaletteMap;
  std::map<std::string, std::shared_ptr<AbstractPalette>> mWPEPaletteMap;
};

#endif /* PALETTEMANAGER_H */
