/*
 * Techdata.cpp
 *
 *      Author: Andreas Volz
 */

#include <dat/Tech.h>
#include "Logger.h"
#include "PropertyNotAvailableException.h"

static Logger logger = Logger("startool.dat.Techdata");

using namespace std;

namespace dat
{

Tech::Tech(DataHub &datahub, unsigned int id) :
  ObjectAccess(datahub, id)
{

}

Tech::~Tech()
{

}

uint16_t Tech::mineral_cost()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->mineral_cost()->at(mId);
}

uint16_t Tech::vespene_cost()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->vespene_cost()->at(mId);
}

uint16_t Tech::research_time()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->research_time()->at(mId);
}

uint16_t Tech::energy_required()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->energy_required()->at(mId);
}

uint32_t Tech::unknown4()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->unknown4()->at(mId);
}

uint16_t Tech::icon()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->icon()->at(mId);
}

uint16_t Tech::label()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->label()->at(mId);
}

TblEntry Tech::label_tbl()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");

  if(label() == Tech::label_none)
  {
    throw PropertyNotAvailableException(mId, "label_tbl");
  }

  return mDatahub.stat_txt_tbl_vec.at(label()-1);
}

uint8_t Tech::race()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->race()->at(mId);
}

uint8_t Tech::unused()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->unused()->at(mId);
}

bool Tech::broodwar_flag()
{
  LOG4CXX_TRACE(logger, to_string(mId) + "=>" + LOG_CUR_FUNC + "()");
  return mDatahub.techdata->broodwar_flag()->at(mId);
}

bool Tech::has_broodwar_flag()
{
  return mDatahub.techdata->has_broodwar_flag();
}

} /* namespace dat */
