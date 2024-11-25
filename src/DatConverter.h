/*
 * DatConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef DATCONVERTER_H
#define DATCONVERTER_H

/* project */
#include "Converter.h"
#include "dat/DataHub.h"
#include "dat/Unit.h"
#include "Storage.h"

/* system */
#include <nlohmann/json.hpp>

// -> units_dat.ksy

void to_json(nlohmann::json &j, units_dat_t::hit_points_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::special_ability_flags_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::staredit_placement_box_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::addon_position_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::unit_dimension_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::staredit_group_flags_type_t* t);
void to_json(nlohmann::json &j, units_dat_t::staredit_availability_flags_type_t* t);

// -> file_tbl.ksy

// -> specific wrapper objects in dat namespace

namespace dat {

void to_json(nlohmann::json &j, TblEntry t);
void to_json(nlohmann::json &j, Upgrade u);
void to_json(nlohmann::json &j, Techdata t);
void to_json(nlohmann::json &j, Order o);
void to_json(nlohmann::json &j, Weapon w);
void to_json(nlohmann::json &j, Sfx s);
void to_json(nlohmann::json &j, Portrait p);
void to_json(nlohmann::json &j, Image i);
void to_json(nlohmann::json &j, Sprite s);
void to_json(nlohmann::json &j, Flingy f);
void to_json(nlohmann::json &j, Unit u);

} /* namespace dat */

class DatConverter : public Converter
{
public:
  DatConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub);
  virtual ~DatConverter();

  void convert(Storage jsonStorage);

private:
  void saveJson(nlohmann::json &j, const std::string &file, bool pretty);

  dat::DataHub &mDatahub;
};

#endif /* DATCONVERTER_H */
