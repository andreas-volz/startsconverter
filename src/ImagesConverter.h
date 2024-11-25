/*
 * ImagesConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef IMAGESCONVERTER_H
#define IMAGESCONVERTER_H

// project
#include "Converter.h"
#include "dat/DataHub.h"
#include "PaletteManager.h"


class ImagesConverter : public Converter
{
public:
  ImagesConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub, PaletteManager &palette_manager);
  virtual ~ImagesConverter();

  void convert(Storage graphics);

private:
  dat::DataHub &mDatahub;
  PaletteManager &mPaletteManager;
};

#endif /* IMAGESCONVERTER_H */
