/*
 * DatConverter.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "DatConverter.h"
#include "FileUtil.h"
#include "dat/Unit.h"

/* system */
#include <fstream>

using namespace std;
using namespace dat;
using json = nlohmann::json;

DatConverter::DatConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub) :
  Converter(hurricane),
  mDatahub(datahub)
{
}

DatConverter::~DatConverter()
{
}

void DatConverter::convert(Storage jsonStorage)
{
  bool pretty = true;

  /**
   * This does control if the JSON generator creates nested objects.
   * If set to true at this point the generated JSON files are big.
   * But you may set some files to true below for debugging...
   */
  bool nested_objects = false;

  // save all global dat files
  jsonStorage.setDataType("dat/");
  CheckPath(jsonStorage);

  json j_units_dat(export_units_dat());
  saveJson(j_units_dat, jsonStorage("units_dat.json"), pretty);

  json j_orders_dat(export_orders_dat());
  saveJson(j_orders_dat, jsonStorage("orders_dat.json"), pretty);

  json j_weapons_dat(export_weapons_dat());
  saveJson(j_weapons_dat, jsonStorage("weapons_dat.json"), pretty);

  json j_flingy_dat(export_flingy_dat());
  saveJson(j_flingy_dat, jsonStorage("flingy_dat.json"), pretty);

  json j_sprites_dat(export_sprites_dat());
  saveJson(j_sprites_dat, jsonStorage("sprites_dat.json"), pretty);

  json j_images_dat(export_images_dat());
  saveJson(j_images_dat, jsonStorage("images_dat.json"), pretty);

  json j_sfxdata_dat(export_sfxdata_dat());
  saveJson(j_sfxdata_dat, jsonStorage("sfxdata_dat.json"), pretty);

  json j_portdata_dat(export_portdata_dat());
  saveJson(j_portdata_dat, jsonStorage("portdata_dat.json"), pretty);

  json j_upgrades_dat(export_upgrades_dat());
  saveJson(j_upgrades_dat, jsonStorage("upgrades_dat.json"), pretty);

  json j_techdata_dat(export_techdata_dat());
  saveJson(j_techdata_dat, jsonStorage("techdata_dat.json"), pretty);

  json j_mapdata_dat(export_mapdata_dat());
  saveJson(j_mapdata_dat, jsonStorage("mapdata_dat.json"), pretty);

  // export all units
  jsonStorage.setDataType("dat/units/");
  CheckPath(jsonStorage);

  for (unsigned int units = 0; units < mDatahub.units->flingy()->size(); units++)
  {
    Unit unit_obj(mDatahub, units, "");
    unit_obj.set_generate_objects(nested_objects);

    json j_unit(unit_obj);
    string num_string = to_string(units);
    saveJson(j_unit, jsonStorage("unit_" + num_string + ".json"), pretty);
  }

  // export all graphics (flingy)
  jsonStorage.setDataType("dat/flingy/");
  CheckPath(jsonStorage);

  for (unsigned int graphics = 0; graphics < mDatahub.flingy->sprite()->size(); graphics++)
  {
    Flingy flingy_obj(mDatahub, graphics);
    flingy_obj.set_generate_objects(nested_objects);

    json j_flingy(flingy_obj);
    string num_string = to_string(graphics);
    saveJson(j_flingy, jsonStorage("flingy_" + num_string + ".json"), pretty);
  }

  // export all sprites
  jsonStorage.setDataType("dat/sprites/");
  CheckPath(jsonStorage);

  for (unsigned int sprite = 0; sprite < mDatahub.sprites->image()->size(); sprite++)
  {
    Sprite sprite_obj(mDatahub, sprite);
    sprite_obj.set_generate_objects(nested_objects);

    json j_sprite(sprite_obj);
    string num_string = to_string(sprite);
    saveJson(j_sprite, jsonStorage("sprite_" + num_string + ".json"), pretty);
  }

  // export all images
  jsonStorage.setDataType("dat/images/");
  CheckPath(jsonStorage);

  for (unsigned int image = 0; image < mDatahub.images->grp()->size(); image++)
  {
    Image image_obj(mDatahub, image);
    image_obj.set_generate_objects(nested_objects);

    json j_image(image_obj);
    string num_string = to_string(image);
    saveJson(j_image, jsonStorage("image_" + num_string + ".json"), pretty);
  }

  /// save all the the stat_txt.tbl parts...
  jsonStorage.setDataType("dat/tbl/");
  CheckPath(jsonStorage);

  json j_images_tbl(mDatahub.images_tbl_vec);
  saveJson(j_images_tbl, jsonStorage("images_tbl.json"), pretty);

  json j_sfxdata_tbl(mDatahub.sfxdata_tbl_vec);
  saveJson(j_sfxdata_tbl, jsonStorage("sfxdata_tbl.json"), pretty);

  json j_portdata_tbl(mDatahub.portdata_tbl_vec);
  saveJson(j_portdata_tbl, jsonStorage("portdata_tbl.json"), pretty);

  json j_mapdata_tbl(mDatahub.mapdata_tbl_vec);
  saveJson(j_mapdata_tbl, jsonStorage("mapdata_tbl.json"), pretty);

  json j_stat_txt_tbl(mDatahub.stat_txt_tbl_vec);
  saveJson(j_stat_txt_tbl, jsonStorage("stat_txt_tbl.json"), pretty);


}

void DatConverter::saveJson(json &j, const std::string &file, bool pretty)
{
  std::ofstream filestream(file);

  if(pretty)
  {
    filestream << std::setw(4) << j;
  }
  else
  {
    filestream << j;
  }
}

/* below are all the nlohmann JSON generator functions */

json DatConverter::export_units_dat()
{
  std::shared_ptr<units_dat_t> units = mDatahub.units;

  json j;

  j["flingy"] = json(*units->flingy());

  j["subunit1"] = json(*units->subunit1());

  j["subunit2"] = json(*units->subunit2());

  j["infestation"] = json(*units->infestation());

  j["construction_animation"] = json(*units->construction_animation());

  j["unit_direction"] = json(*units->unit_direction());

  j["shield_enable"] = json(*units->shield_enable());

  j["shield_amount"] = json(*units->shield_amount());

  j["hit_points"] = json(*units->hit_points());

  j["elevation_level"] = json(*units->elevation_level());

  j["unknown"] = json(*units->unknown());

  j["rank"] = json(*units->rank());

  j["ai_computer_idle"] = json(*units->ai_computer_idle());

  j["ai_human_idle"] = json(*units->ai_human_idle());

  j["ai_return_to_idle"] = json(*units->ai_return_to_idle());

  j["ai_attack_unit"] = json(*units->ai_attack_unit());

  j["ai_attack_move"] = json(*units->ai_attack_move());

  j["ground_weapon"] = json(*units->ground_weapon());

  if(units->is_format_bw())
  {
    j["max_ground_hits"] = json(*units->max_ground_hits());
  }

  j["air_weapon"] = json(*units->air_weapon());

  if(units->is_format_bw())
  {
    j["max_air_hits"] = json(*units->max_air_hits());
  }

  j["ai_internal"] = json(*units->ai_internal());

  j["special_ability_flags"] = json(*units->special_ability_flags());

  j["target_acquisition_range"] = json(*units->target_acquisition_range());

  j["sight_range"] = json(*units->sight_range());

  j["armor_upgrade"] = json(*units->armor_upgrade());

  j["unit_size"] = json(*units->unit_size());

  j["armor"] = json(*units->armor());

  j["right_click_action"] = json(*units->right_click_action());

  j["ready_sound"] = json(*units->ready_sound());

  j["what_sound_start"] = json(*units->what_sound_start());

  j["what_sound_end"] = json(*units->what_sound_end());

  j["piss_sound_start"] = json(*units->piss_sound_start());

  j["piss_sound_end"] = json(*units->piss_sound_end());

  j["yes_sound_start"] = json(*units->yes_sound_start());

  j["yes_sound_end"] = json(*units->yes_sound_end());

  j["staredit_placement_box"] = json(*units->staredit_placement_box());

  j["addon_position"] = json(*units->addon_position());

  j["unit_dimension"] = json(*units->unit_dimension());

  j["portrait"] = json(*units->portrait());

  j["mineral_cost"] = json(*units->mineral_cost());

  j["vespene_cost"] = json(*units->vespene_cost());

  j["build_time"] = json(*units->build_time());

  j["requirements"] = json(*units->requirements());

  j["group_flags"] = json(*units->group_flags());

  j["supply_provided"] = json(*units->supply_provided());

  j["supply_required"] = json(*units->supply_required());

  j["space_required"] = json(*units->space_required());

  j["space_provided"] = json(*units->space_provided());

  j["build_score"] = json(*units->build_score());

  j["destroy_score"] = json(*units->destroy_score());

  j["unit_map_string"] = json(*units->unit_map_string());

  if(units->is_format_bw())
  {
    j["broodwar_flag"] = json(*units->broodwar_flag());
  }

  j["is_format_bw"] = json(units->is_format_bw());

  j["staredit_availability_flags"] = json(*units->staredit_availability_flags());

  return j;
}

json DatConverter::export_orders_dat()
{
  std::shared_ptr<orders_dat_t> orders = mDatahub.orders;

  json j;

  j["label"] = json(*orders->label());

  j["use_weapon_targeting"] = json(*orders->use_weapon_targeting());

  j["unknown2"] = json(*orders->unknown2());

  j["unknown3"] = json(*orders->unknown3());

  j["unknown4"] = json(*orders->unknown4());

  j["unknown5"] = json(*orders->unknown5());

  j["interruptible"] = json(*orders->interruptible());

  j["unknown7"] = json(*orders->unknown7());

  j["queueable"] = json(*orders->queueable());

  j["unknown9"] = json(*orders->unknown9());

  j["unknown10"] = json(*orders->unknown10());

  j["unknown11"] = json(*orders->unknown11());

  j["unknown12"] = json(*orders->unknown12());

  j["targeting"] = json(*orders->targeting());

  j["energy"] = json(*orders->energy());

  j["animation"] = json(*orders->animation());

  j["highlight"] = json(*orders->highlight());

  j["unknown17"] = json(*orders->unknown17());

  j["obscured_order"] = json(*orders->obscured_order());

  return j;
}

json DatConverter::export_weapons_dat()
{
  std::shared_ptr<weapons_dat_t> weapons = mDatahub.weapons;

  json j;

  j["label"] = json(*weapons->label());

  j["flingy"] = json(*weapons->flingy());

  j["explosion"] = json(*weapons->explosion());

  j["target_flags"] = json(*weapons->target_flags());

  j["minimum_range"] = json(*weapons->minimum_range());

  j["maximum_range"] = json(*weapons->maximum_range());

  j["damage_upgrade"] = json(*weapons->damage_upgrade());

  j["weapon_type"] = json(*weapons->weapon_type());

  j["weapon_behaviour"] = json(*weapons->weapon_behaviour());

  j["remove_after"] = json(*weapons->remove_after());

  j["explosive_type"] = json(*weapons->explosive_type());

  j["inner_splash_range"] = json(*weapons->inner_splash_range());

  j["medium_splash_range"] = json(*weapons->medium_splash_range());

  j["outer_splash_range"] = json(*weapons->outer_splash_range());

  j["damage_amount"] = json(*weapons->damage_amount());

  j["damage_bonus"] = json(*weapons->damage_bonus());

  j["weapon_cooldown"] = json(*weapons->weapon_cooldown());

  j["damage_factor"] = json(*weapons->damage_factor());

  j["attack_angle"] = json(*weapons->attack_angle());

  j["launch_spin"] = json(*weapons->launch_spin());

  j["x_offset"] = json(*weapons->x_offset());

  j["y_offset"] = json(*weapons->y_offset());

  j["error_message"] = json(*weapons->error_message());

  j["icon"] = json(*weapons->icon());

  return j;
}

json DatConverter::export_flingy_dat()
{
  std::shared_ptr<flingy_dat_t> flingy = mDatahub.flingy;

  json j;

  j["sprite"] = json(*flingy->sprite());

  j["speed"] = json(*flingy->speed());

  j["acceleration"] = json(*flingy->acceleration());

  j["halt_distance"] = json(*flingy->halt_distance());

  j["turn_radius"] = json(*flingy->turn_radius());

  j["unused"] = json(*flingy->unused());

  j["movement_control"] = json(*flingy->movement_control());

  return j;
}

json DatConverter::export_sprites_dat()
{
  std::shared_ptr<sprites_dat_t> sprites = mDatahub.sprites;

  json j;

  j["image"] = json(*sprites->image());

  j["health_bar"] = json(*sprites->health_bar());

  j["unknown2"] = json(*sprites->unknown2());

  j["is_visible"] = json(*sprites->is_visible());

  j["select_circle_image_size"] = json(*sprites->select_circle_image_size());

  j["select_circle_vertical_pos"] = json(*sprites->select_circle_vertical_pos());

  return j;
}

json DatConverter::export_images_dat()
{
  std::shared_ptr<images_dat_t> images = mDatahub.images;

  json j;

  j["grp"] = json(*images->grp());

  j["gfx_turns"] = json(*images->gfx_turns());

  j["clickable"] = json(*images->clickable());

  j["use_full_iscript"] = json(*images->use_full_iscript());

  j["draw_if_cloaked"] = json(*images->draw_if_cloaked());

  j["draw_function"] = json(*images->draw_function());

  j["remapping"] = json(*images->remapping());

  j["iscript"] = json(*images->iscript());

  j["shield_overlay"] = json(*images->shield_overlay());

  j["attack_overlay"] =  json(*images->attack_overlay());

  j["damage_overlay"] = json(*images->damage_overlay());

  j["special_overlay"] = json(*images->special_overlay());

  j["landing_dust_overlay"] = json(*images->landing_dust_overlay());

  j["lift_off_dust_overlay"] = json(*images->lift_off_dust_overlay());

  return j;
}

json DatConverter::export_sfxdata_dat()
{
  std::shared_ptr<sfxdata_dat_t> sfxdata = mDatahub.sfxdata;

  json j;

  j["sound_file"] = json(*sfxdata->sound_file());

  j["unknown1"] = json(*sfxdata->unknown1());

  j["unknown2"] = json(*sfxdata->unknown2());

  j["unknown3"] = json(*sfxdata->unknown3());

  j["unknown4"] = json(*sfxdata->unknown4());

  return j;
}

json DatConverter::export_portdata_dat()
{
  std::shared_ptr<portdata_dat_t> portdata = mDatahub.portdata;

  json j;

  j["video_idle"] = json(*portdata->video_idle());

  j["video_talking"] = json(*portdata->video_talking());

  j["change_idle"] = json(*portdata->change_idle());

  j["change_talking"] = json(*portdata->change_talking());

  j["unknown1_idle"] = json(*portdata->unknown1_idle());

  j["unknown1_talking"] = json(*portdata->unknown1_talking());

  return j;
}

json DatConverter::export_upgrades_dat()
{
  std::shared_ptr<upgrades_dat_t> upgrades = mDatahub.upgrades;

  json j;

  j["mineral_cost_base"] = json(*upgrades->mineral_cost_base());

  j["mineral_cost_factor"] = json(*upgrades->mineral_cost_factor());

  j["vespene_cost_base"] = json(*upgrades->vespene_cost_base());

  j["vespene_cost_factor"] = json(*upgrades->vespene_cost_factor());

  j["research_time_base"] = json(*upgrades->research_time_base());

  j["research_time_factor"] = json(*upgrades->research_time_factor());

  j["unknown6"] = json(*upgrades->unknown6());

  j["icon"] = json(*upgrades->icon());

  j["label"] = json(*upgrades->label());

  j["race"] = json(*upgrades->race());

  j["max_repeats"] = json(*upgrades->max_repeats());

  if(upgrades->has_broodwar_flag())
  {
    j["broodwar_flags"] = json(*upgrades->broodwar_flags());
  }

  return j;
}

json DatConverter::export_techdata_dat()
{
  std::shared_ptr<techdata_dat_t> techdata = mDatahub.techdata;

  json j;

  j["mineral_cost"] = json(*techdata->mineral_cost());

  j["vespene_cost"] = json(*techdata->vespene_cost());

  j["research_time"] = json(*techdata->research_time());

  j["energy_required"] = json(*techdata->energy_required());

  j["unknown4"] = json(*techdata->unknown4());

  j["icon"] = json(*techdata->icon());

  j["label"] = json(*techdata->label());

  j["race"] = json(*techdata->race());

  j["unused"] = json(*techdata->unused());

  if(techdata->has_broodwar_flag())
  {
    j["broodwar_flag"] = json(*techdata->broodwar_flag());
  }

  return j;
}

json DatConverter::export_mapdata_dat()
{
  std::shared_ptr<mapdata_dat_t> mapdata = mDatahub.mapdata;

  json j;

  j["mission_dir"] = json(*mapdata->mission_dir());

  return j;
}

void to_json(json &j, units_dat_t::hit_points_type_t *t)
{
  // TODO: maybe I save hitpoints0() and hitpoints1() later in two values if I know better why it was needed!
  j = json(t->hitpoints0() + t->hitpoints1());
}

void to_json(json &j, units_dat_t::special_ability_flags_type_t *t)
{
  j = json
  {
    {"building", t->building()},
    {"addon", t->addon()},
    {"flyer", t->flyer()},
    {"resourceminer", t->resourceminer()},
    {"subunit", t->subunit()},
    {"flyingbuilding", t->flyingbuilding()},
    {"hero", t->hero()},
    {"regenerate", t->regenerate()},
    {"animatedidle", t->animatedidle()},
    {"cloakable", t->cloakable()},
    {"twounitsinoneegg", t->twounitsinoneegg()},
    {"singleentity", t->singleentity()},
    {"resourcedepot", t->resourcedepot()},
    {"resourcecontainter", t->resourcecontainter()},
    {"robotic", t->robotic()},
    {"detector", t->detector()},
    {"organic", t->organic()},
    {"requirescreep", t->requirescreep()},
    {"unused", t->unused()},
    {"requirespsi", t->requirespsi()},
    {"burrowable", t->burrowable()},
    {"spellcaster", t->spellcaster()},
    {"permanentcloak", t->permanentcloak()},
    {"pickupitem", t->pickupitem()},
    {"ignoresupplycheck", t->ignoresupplycheck()},
    {"usemediumoverlays", t->usemediumoverlays()},
    {"uselargeoverlays", t->uselargeoverlays()},
    {"battlereactions", t->battlereactions()},
    {"fullautoattack", t->fullautoattack()},
    {"invincible", t->invincible()},
    {"mechanical", t->mechanical()},
    {"producesunits", t->producesunits()}
  };
}

void to_json(json &j, units_dat_t::staredit_placement_box_type_t *t)
{
  j = json{ {"width", t->width()}, {"height", t->height()} };
}

void to_json(json &j, units_dat_t::addon_position_type_t *t)
{
  j = json{ {"horizontal", t->horizontal()}, {"vertical", t->vertical()} };
}

void to_json(json &j, units_dat_t::unit_dimension_type_t *t)
{
  j = json{ {"left", t->left()}, {"up", t->up()}, {"right", t->right()}, {"down", t->down()} };
}

void to_json(json &j, units_dat_t::group_flags_type_t *t)
{
  j = json
  {
    {"zerg", t->zerg()},
    {"terran", t->terran()},
    {"protoss", t->protoss()},
    {"men", t->men()},
    {"building", t->building()},
    {"factory", t->factory()},
    {"independent", t->independent()},
    {"neutral", t->neutral()}
  };
}

void to_json(json &j, units_dat_t::staredit_availability_flags_type_t *t)
{
  j = json
  {
    {"non_neutral", t->non_neutral()},
    {"unit_listing", t->unit_listing()},
    {"mission_briefing", t->mission_briefing()},
    {"player_settings", t->player_settings()},
    {"all_races", t->all_races()},
    {"set_doodad_state", t->set_doodad_state()},
    {"non_location_triggers", t->non_location_triggers()},
    {"unit_hero_settings", t->unit_hero_settings()},
    {"location_triggers", t->location_triggers()},
    {"brood_war_only", t->brood_war_only()}
  };
}

namespace dat
{

void to_json(json &j, TblEntry t)
{
  j = json
  {
    {"name1", t.name1()},
    {"name2", t.name2()},
    {"name3", t.name3()},
    {"shortcut_pos", t.shortcut_pos()},
    {"shortcut", t.shortcut()}
  };
}

void to_json(json &j, Upgrade u)
{
  j["id"] = json(u.id());
  j["mineral_cost_base"] = json(u.mineral_cost_base());
  j["mineral_cost_factor"] = json(u.mineral_cost_factor());
  j["vespene_cost_base"] = json(u.vespene_cost_base());
  j["vespene_cost_factor"] = json(u.vespene_cost_factor());
  j["research_time_base"] = json(u.research_time_base());
  j["research_time_factor"] = json(u.research_time_factor());
  j["unknown6"] = json(u.unknown6());
  j["icon"] = json(u.icon());
  j["label"] = json(u.label());
  j["label_tbl"] = json(u.label_tbl());
  j["race"] = json(u.race());
  j["max_repeats"] = json(u.max_repeats());

  if (u.has_broodwar_flag())
  {
    j["broodwar_flags"] = json(u.broodwar_flags());
  }
}

void to_json(json &j, Order o)
{
  j["id"] = json(o.id());
  j["label"] = json(o.label());
  if(o.get_generate_objects())
  {
    j["label_tbl"] = json(o.label_tbl());
  }
  j["use_weapon_targeting"] = json(o.use_weapon_targeting());
  j["unknown2"] = json(o.unknown2());
  j["unknown3"] = json(o.unknown3());
  j["unknown4"] = json(o.unknown4());
  j["unknown5"] = json(o.unknown5());
  j["interruptible"] = json(o.interruptible());
  j["unknown7"] = json(o.unknown7());
  j["queueable"] = json(o.queueable());
  j["unknown9"] = json(o.unknown9());
  j["unknown10"] = json(o.unknown10());
  j["unknown11"] = json(o.unknown11());
  j["unknown12"] = json(o.unknown12());

  j["targeting"] = json(o.targeting());
  if(o.get_generate_objects())
  {
    try
    {
      j["targeting_obj"] = json(o.targeting_obj());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }
  j["energy"] = json(o.energy());
  if(o.get_generate_objects())
    {
    try
    {
      j["energy_obj"] = json(o.energy_obj());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  j["animation"] = json(o.animation());
  j["highlight"] = json(o.highlight());
  j["unknown17"] = json(o.unknown17());
  j["obscured_order"] = json(o.obscured_order());
  //j["obscured_order_obj"] = json(o.obscured_order_obj());
}

void to_json(json &j, Tech t)
{
  j["id"] = json(t.id());
  j["mineral_cost"] = json(t.mineral_cost());
  j["vespene_cost"] = json(t.vespene_cost());
  j["research_time"] = json(t.research_time());
  j["energy_required"] = json(t.energy_required());
  j["unknown4"] = json(t.unknown4());
  j["icon"] = json(t.icon());
  j["label"] = json(t.label());
  j["race"] = json(t.race());
  j["unused"] = json(t.unused());

  if (t.has_broodwar_flag())
  {
    j["broodwar_flag"] = json(t.broodwar_flag());
  }
}

void to_json(json &j, Weapon w)
{
  j["id"] = json(w.id());
  j["label"] = json(w.label());
  if(w.get_generate_objects())
  {
    j["label_tbl"] = json(w.label_tbl());
  }
  j["flingy"] = json(w.flingy());
  if(w.get_generate_objects())
  {
    j["flingy_obj"] = json(w.flingy_obj());
  }
  j["explosion"] = json(w.explosion());
  j["target_flags"] = json(w.target_flags());
  j["minimum_range"] = json(w.minimum_range());
  j["maximum_range"] = json(w.maximum_range());
  j["damage_upgrade"] = json(w.damage_upgrade());
  if(w.get_generate_objects())
  {
    j["damage_upgrade_obj"] = json(w.damage_upgrade_obj());
  }
  j["weapon_type"] = json(w.weapon_type());
  j["weapon_behaviour"] = json(w.weapon_behaviour());
  j["remove_after"] = json(w.remove_after());
  j["explosive_type"] = json(w.explosive_type());
  j["inner_splash_range"] = json(w.inner_splash_range());
  j["medium_splash_range"] = json(w.medium_splash_range());
  j["outer_splash_range"] = json(w.outer_splash_range());
  j["damage_amount"] = json(w.damage_amount());
  j["damage_bonus"] = json(w.damage_bonus());
  j["weapon_cooldown"] = json(w.weapon_cooldown());
  j["damage_factor"] = json(w.damage_factor());
  j["attack_angle"] = json(w.attack_angle());
  j["launch_spin"] = json(w.launch_spin());
  j["x_offset"] = json(w.x_offset());
  j["y_offset"] = json(w.y_offset());
  j["error_message"] = json(w.error_message());
  if(w.get_generate_objects())
  {
    j["error_message_tbl"] = json(w.error_message_tbl());
  }
  j["icon"] = json(w.icon());
}

void to_json(json &j, Sfx s)
{
  j["id"] = json(s.id());
  j["sound_file"] = json(s.sound_file());
  if(s.get_generate_objects())
  {
    j["sound_file_tbl"] = json(s.sound_file_tbl());
  }
  j["unknown1"] = json(s.unknown1());
  j["unknown2"] = json(s.unknown2());
  j["unknown3"] = json(s.unknown3());
  j["unknown4"] = json(s.unknown4());
}

void to_json(json &j, Portrait p)
{
  j["id"] = json(p.id());
  j["video_idle"] = json(p.video_idle());
  if(p.get_generate_objects())
  {
    j["video_idle_tbl"] = json(p.video_idle_tbl());
  }
  j["video_talking"] = json(p.video_talking());
  if(p.get_generate_objects())
  {
    j["video_talking_tbl"] = json(p.video_talking_tbl());
  }
  j["change_idle"] = json(p.change_idle());
  j["change_talking"] = json(p.change_talking());
  j["unknown1_idle"] = json(p.unknown1_idle());
  j["unknown1_talking"] = json(p.unknown1_talking());
}

void to_json(json &j, Image i)
{
  j["id"] = json(i.id());
  j["grp"] = json(i.grp());
  if(i.get_generate_objects())
  {
    j["grp_tbl"] = json(i.grp_tbl());
  }
  j["gfx_turns"] = json(i.gfx_turns());
  j["clickable"] = json(i.clickable());
  j["use_full_iscript"] = json(i.use_full_iscript());
  j["draw_if_cloaked"] = json(i.draw_if_cloaked());
  j["draw_function"] = json(i.draw_function());
  j["remapping"] = json(i.remapping());
  j["iscript"] = json(i.iscript());
  j["shield_overlay"] = json(i.shield_overlay());
  if(i.get_generate_objects())
  {
    try
    {
      j["shield_overlay_tbl"] = json(i.shield_overlay_tbl());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  j["attack_overlay"] = json(i.attack_overlay());
  if(i.get_generate_objects())
  {
    try
    {
      j["attack_overlay_tbl"] = json(i.attack_overlay_tbl());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  j["damage_overlay"] = json(i.damage_overlay());
  if(i.get_generate_objects())
  {
    try
    {
      j["damage_overlay_tbl"] = json(i.damage_overlay_tbl());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }
  j["special_overlay"] = json(i.special_overlay());
  if(i.get_generate_objects())
  {
    try
    {
      j["special_overlay_tbl"] = json(i.special_overlay_tbl());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  j["landing_dust_overlay"] = json(i.landing_dust_overlay());
  if(i.get_generate_objects())
  {
    try
    {
      j["landing_dust_overlay_tbl"] = json(i.landing_dust_overlay_tbl());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }
}

void to_json(json &j, Sprite s)
{
  j["id"] = json(s.id());
  j["image"] = json(s.image());
  if(s.get_generate_objects())
  {
    j["image_obj"] = json(s.image_obj());
  }

  try
  {
    j["health_bar"] = json(s.health_bar());
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  j["unknown2"] = json(s.unknown2());
  j["is_visible"] = json(s.is_visible());

  try
  {
    j["select_circle_image_size"] = json(s.select_circle_image_size());
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["select_circle_vertical_pos"] = json(s.select_circle_vertical_pos());
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }
}

void to_json(json &j, Flingy f)
{
  j["id"] = json(f.id());
  j["sprite"] = json(f.sprite());
  if(f.get_generate_objects())
  {
    j["sprite_obj"] = json(f.sprite_obj());
  }
  j["speed"] = json(f.speed());
  j["acceleration"] = json(f.acceleration());
  j["halt_distance"] = json(f.halt_distance());
  j["turn_radius"] = json(f.turn_radius());
  j["unused"] = json(f.unused());
  j["movement_control"] = json(f.movement_control());

}

void to_json(json &j, Unit u)
{
  j["id"] = json(u.id());
  j["id_string"] = json(u.getIDString());
  if(u.get_generate_objects())
  {
    j["name_tbl"] = json(u.name_tbl());
  }
  j["flingy"] = json(u.flingy());
  if(u.get_generate_objects())
  {
    j["flingy_obj"] = json(u.flingy_obj());
  }
  j["subunit1"] = json(u.subunit1());
  if(u.get_generate_objects())
  {
    try
    {
      j["subunit1_obj"] = json(u.subunit1_obj());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  j["subunit2"] = json(u.subunit2());

  if(u.get_generate_objects())
  {
    try
    {
      j["subunit2_obj"] = json(u.subunit2_obj());
    }
    catch (PropertyNotAvailableException &ex)
    { /*it's fine, do nothing */ }
  }

  try
  {
    j["infestation"] = json(u.infestation());
    if(u.get_generate_objects())
    {
      j["infestation_obj"] = json(u.infestation_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["construction_animation"] = json(u.construction_animation());
    if(u.get_generate_objects())
    {
      j["construction_animation_obj"] = json(u.construction_animation_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  j["unit_direction"] = json(u.unit_direction());
  j["shield_enable"] = json(u.shield_enable());
  j["shield_amount"] = json(u.shield_amount());
  j["hitpoints"] = json(u.hit_points());
  j["elevation_level"] = json(u.elevation_level());
  j["unknown"] = json(u.unknown());
  j["rank"] = json(u.rank());
  j["ai_computer_idle"] = json(u.ai_computer_idle());
  if(u.get_generate_objects())
  {
    j["ai_computer_idle_obj"] = json(u.ai_computer_idle_obj());
  }
  j["ai_human_idle"] = json(u.ai_human_idle());
  if(u.get_generate_objects())
  {
    j["ai_human_idle_obj"] = json(u.ai_human_idle_obj());
  }
  j["ai_return_to_idle"] = json(u.ai_return_to_idle());
  if(u.get_generate_objects())
  {
    j["ai_return_to_idle_obj"] = json(u.ai_return_to_idle_obj());
  }
  j["ai_attack_unit"] = json(u.ai_attack_unit());
  if(u.get_generate_objects())
  {
    j["ai_attack_unit_obj"] = json(u.ai_attack_unit_obj());
  }
  j["ai_attack_move"] = json(u.ai_attack_move());
  if(u.get_generate_objects())
  {
    j["ai_attack_move_obj"] = json(u.ai_attack_move_obj());
  }

  try
  {
    j["ground_weapon"] = json(u.ground_weapon());
    if(u.get_generate_objects())
    {
      j["ground_weapon_obj"] = json(u.ground_weapon_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  if (u.is_format_bw())
  {
    j["max_ground_hits"] = json(u.max_ground_hits());
  }

  try
  {
    j["air_weapon"] = json(u.air_weapon());
    if(u.get_generate_objects())
    {
      j["air_weapon_obj"] = json(u.air_weapon_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  if (u.is_format_bw())
  {
    j["max_air_hits"] = json(u.max_air_hits());
  }

  j["ai_internal"] = json(u.ai_internal());
  j["special_ability_flags"] = json(u.special_ability_flags());
  j["target_acquisition_range"] = json(u.target_acquisition_range());
  j["sight_range"] = json(u.sight_range());
  j["armor_upgrade"] = json(u.armor_upgrade());
  j["unit_size"] = json(u.unit_size());
  j["armor"] = json(u.armor());
  j["right_click_action"] = json(u.right_click_action());

  try
  {
    j["ready_sound"] = json(u.ready_sound());
    if(u.get_generate_objects())
    {
      j["ready_sound_obj"] = json(u.ready_sound_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["what_sound_start"] = json(u.what_sound_start());
    j["what_sound_end"] = json(u.what_sound_end());
    if(u.get_generate_objects())
    {
      j["what_sound_obj"] = json(u.what_sound_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["piss_sound_start"] = json(u.piss_sound_start());
    j["piss_sound_end"] = json(u.piss_sound_end());
    if(u.get_generate_objects())
    {
      j["piss_sound_obj"] = json(u.piss_sound_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["what_sound_start"] = json(u.what_sound_start());
    j["what_sound_end"] = json(u.what_sound_end());
    if(u.get_generate_objects())
    {
      j["what_sound_obj"] = json(u.what_sound_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  try
  {
    j["yes_sound_start"] = json(u.yes_sound_start());
    j["yes_sound_end"] = json(u.yes_sound_end());
    if(u.get_generate_objects())
    {
      j["yes_sound_obj"] = json(u.yes_sound_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  j["staredit_placement_box"] = json(u.staredit_placement_box());

  try
  {
    j["addon_position"] = json(u.addon_position());
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  j["unit_dimension"] = json(u.unit_dimension());


  try
  {
    j["portrait"] = json(u.portrait());
    if(u.get_generate_objects())
    {
      j["portrait_obj"] = json(u.portrait_obj());
    }
  }
  catch (PropertyNotAvailableException &ex)
  { /*it's fine, do nothing */ }

  j["mineral_cost"] = json(u.mineral_cost());
  j["vespene_cost"] = json(u.vespene_cost());
  j["build_time"] = json(u.build_time());
  j["requirements"] = json(u.requirements());
  j["group_flags"] = json(u.group_flags());
  j["supply_provided"] = json(u.supply_provided());
  j["supply_required"] = json(u.supply_required());
  j["space_provided"] = json(u.space_provided());
  j["space_required"] = json(u.space_required());
  j["build_score"] = json(u.build_score());
  j["destroy_score"] = json(u.destroy_score());
  j["unit_map_string"] = json(u.unit_map_string());

  if (u.is_format_bw())
  {
    j["broodwar_flag"] = json(u.broodwar_flag());
  }

  j["is_format_bw"] = json(u.is_format_bw());
}

} /* namespace dat */
