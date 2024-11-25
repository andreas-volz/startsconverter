/*
 * PortraitsConverter.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "PortraitsConverter.h"
#include "dat/Portrait.h"
#include "Smacker.h"
#include "StringUtil.h"
#include "FileUtil.h"
#include "platform.h"
#include "Logger.h"

// system
#include <iostream>
#include <fstream>
#include <set>

using namespace std;
using namespace dat;

static Logger mLogger = Logger("startool.PortraitsConverter");

PortraitsConverter::PortraitsConverter(std::shared_ptr<Hurricane> hurricane, DataHub &datahub) :
    Converter(hurricane),
    mDatahub(datahub)
{
}

PortraitsConverter::~PortraitsConverter()
{
}

bool PortraitsConverter::convert(Storage videosStorage)
{
  bool result = true;


  for(unsigned int i = 0; i < mDatahub.portdata->video_idle()->size(); i++)
  {
    Portrait portrait(mDatahub, i);

    string portrait_arcfile_idle(portrait.video_idle_tbl().name1());
    string portrait_arcfile_talking(portrait.video_talking_tbl().name1());

    string portrait_idle_id(portrait.getIDString(portrait_arcfile_idle));
    string portrait_talking_id(portrait.getIDString(portrait_arcfile_talking));

    // just to ensure the idle / talking consistency in the database
    // this should also be the case otherwise something is broken (TODO: => not sure if this is still valid)
    if(portrait_idle_id == portrait_talking_id)
    {
      vector<string> portrait_list;

      /* TODO: if it'S later needed to write a JSON file this might be a good place */
      convertOgvPortraits(portrait_arcfile_idle, portrait_list, videosStorage);
      convertOgvPortraits(portrait_arcfile_talking, portrait_list, videosStorage);
    }
    else
    {
      LOG4CXX_FATAL(mLogger, "portrait_idle_id != portrait_talking_id");
    }
  }

  return result;
}

bool PortraitsConverter::convertOgvPortraits(const std::string &arcfile, std::vector<std::string> &portrait_list, Storage videosStorage)
{
  bool smk_available = true;
  unsigned int smk_num = 0;

  while(smk_available && smk_num <= 3)
  {
    Smacker video(mHurricane);

    // build the name of the specific #smk file
    string smk_arcfile("portrait\\" + arcfile + to_string(smk_num) + ".smk");

    // target place where to store the file
    string target_basename(arcfile + to_string(smk_num));
    replaceString("\\", "/", target_basename);
    target_basename = to_lower(target_basename);

    LOG4CXX_INFO(mLogger, "Try export (last one may fail if less then three) " + smk_arcfile + " to " + target_basename);
    smk_available = video.convert(smk_arcfile, videosStorage(target_basename));
    LOG4CXX_INFO(mLogger, (smk_available ? "ok" : "nok"));

    if(smk_available)
    {
      portrait_list.push_back(videosStorage.getDataType() + "/" + target_basename + ".ogv");
    }

    smk_num++;
  }

  return true;
}
