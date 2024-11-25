/*
 * SfxConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef SFXCONVERTER_H
#define SFXCONVERTER_H

// project
#include "dat/DataHub.h"
#include "Storage.h"

class SfxConverter : public Converter
{
public:
  SfxConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub);
  virtual ~SfxConverter();

  void convert(Storage sounds);

private:
  dat::DataHub &mDatahub;
};

#endif /* SFXCONVERTER_H */
