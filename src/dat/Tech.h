/*
 * Techdata.h
 *
 *      Author: Andreas
 */

#ifndef TECHDATA_H
#define TECHDATA_H

#include "ObjectAccess.h"

namespace dat
{

class Tech : public ObjectAccess
{
public:
  Tech(DataHub &datahub, unsigned int id);
  virtual ~Tech();

  uint16_t mineral_cost();

  uint16_t vespene_cost();

  uint16_t research_time();

  uint16_t energy_required();

  uint32_t unknown4();

  uint16_t icon();

  uint16_t label();
  TblEntry label_tbl();

  techdata_dat_t::race_enum_t race();

  uint8_t unused();

  bool broodwar_flag();

  bool has_broodwar_flag();

  /* constants */
  static const int label_none = 0;
};

} /* namespace dat */

#endif /* TECHDATA_H */
