/*
 * CampaignConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef CAMPAIGNCONVERTER_H
#define CAMPAIGNCONVERTER_H

/* project */
#include "Converter.h"
#include "Storage.h"
#include "dat/DataHub.h"
#include "TilesetConverter.h"

class CampaignConverter : public Converter
{
public:
  CampaignConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub, TilesetConverter &tilesetConverter);
  virtual ~CampaignConverter();

  void convert(Storage campaignStorage);

private:
  dat::DataHub &mDatahub;
  TilesetConverter &mTilesetConverter;
};

#endif /* CAMPAIGNCONVERTER_H */
