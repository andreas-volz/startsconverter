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


// -> specific wrapper objects in dat namespace
namespace dat {

void to_json(nlohmann::json &j, TblEntry t);
void to_json(nlohmann::json &j, Upgrade u);
void to_json(nlohmann::json &j, Tech t);
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

  nlohmann::json export_units_dat();
  nlohmann::json export_orders_dat();
  nlohmann::json export_weapons_dat();
  nlohmann::json export_flingy_dat();
  nlohmann::json export_sprites_dat();
  nlohmann::json export_images_dat();
  nlohmann::json export_sfxdata_dat();
  nlohmann::json export_portdata_dat();
  nlohmann::json export_upgrades_dat();
  nlohmann::json export_techdata_dat();
  nlohmann::json export_mapdata_dat();

  dat::DataHub &mDatahub;
};

#endif /* DATCONVERTER_H */
