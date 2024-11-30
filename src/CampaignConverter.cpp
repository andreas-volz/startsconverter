/*
 * CampaignConverter.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "CampaignConverter.h"
#include "Chk.h"

/* system */
#include <iostream>

using namespace std;
using namespace dat;

CampaignConverter::CampaignConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub, TilesetConverter &tilesetConverter) :
    Converter(hurricane),
    mDatahub(datahub),
    mTilesetConverter(tilesetConverter)
{
}

CampaignConverter::~CampaignConverter()
{
}

void CampaignConverter::convert(Storage campaignStorage)
{
  for (unsigned int i = 0; i < mDatahub.mapdata->mission_dir()->size(); i++)
  {
    uint32_t mission_dir = mDatahub.mapdata->mission_dir()->at(i);

    TblEntry mission_tbl = mDatahub.mapdata_tbl_vec.at(mission_dir-1);
    string campaign_path(mission_tbl.name1());
    if(campaign_path != "null")
    {
      /*
       * There's only a working terran tutorial in the data.
       * If you unpack the MPQ there's also a protoss and zerg tutorial, but the files seem broken.
       * Maybe old not deleted one. They're also not available in the game. Just ignore them.
       */
      Chk chk(mHurricane, campaign_path);
      chk.convertTiled(*mTilesetConverter.getTilesetHub(chk.getTileSet()), campaignStorage);
      chk.generateMapJson(*mTilesetConverter.getTilesetHub(chk.getTileSet()), campaignStorage);
      //cout << "Campaign: " << campaign_path << " : " << chk.getTileSet() <<  endl;
    }

    // TODO: + protoss and zerg tutorial??
  }
}
