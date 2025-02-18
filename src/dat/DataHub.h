/*
 * DataHub.h
 *
 *      Author: Andreas Volz
 */

#ifndef DATAHUB_H
#define DATAHUB_H

// project
#include "Palette.h"
#include "Palette2D.h"
#include "Tbl.h"

#include "kaitai/units_dat.h"
#include "kaitai/weapons_dat.h"
#include "kaitai/flingy_dat.h"
#include "kaitai/sprites_dat.h"
#include "kaitai/images_dat.h"
#include "kaitai/sfxdata_dat.h"
#include "kaitai/portdata_dat.h"
#include "kaitai/upgrades_dat.h"
#include "kaitai/orders_dat.h"
#include "kaitai/techdata_dat.h"
#include "kaitai/mapdata_dat.h"

// System
#include <map>

namespace dat
{

/**
 * The DataHub parses the complete data structures inside those files:
 * - arr\\units.dat
 * - arr\\orders.dat
 * - arr\\weapons.dat
 * - arr\\flingy.dat
 * - arr\\sprites.dat
 * - arr\\images.dat
 * - arr\\sfxdata.dat
 * - arr\\portdata.dat
 * - arr\\upgrades.dat
 * - arr\\techdata.dat
 * - arr\\mapdata.dat
 * - arr\\images.tbl
 * - arr\\sfxdata.tbl
 * - arr\\portdata.tbl
 * - arr\\mapdata.tbl
 * - rez\\stat_txt.tbl
 *
 * As those data has have depends on each other they're all parsed together in one run. After being parsed
 * to Kaitai data structures they could be accessed from outside.
 *
 * The Kaitai parsed objects in this class are by intension public. There's no benefit in putting dozen silly
 * getter around them. The alternative to put them private means to implement every parser logic
 * inside this class or friend them all.
 *
 * So the design decision is just to let them public and the outside accessing function should only read them!
 * If I ever put this stuff to a general purpose library this design might change.
 *
 *  "with great power comes great responsibility" - (Spiderman)
 */
class DataHub : public Converter
{
public:
  DataHub(std::shared_ptr<Hurricane> hurricane);
  virtual ~DataHub();

  // Kaitai parsed objects
  std::shared_ptr<units_dat_t> units;
  std::shared_ptr<orders_dat_t> orders;
  std::shared_ptr<weapons_dat_t> weapons;
  std::shared_ptr<flingy_dat_t> flingy;
  std::shared_ptr<sprites_dat_t> sprites;
  std::shared_ptr<images_dat_t> images;
  std::shared_ptr<sfxdata_dat_t> sfxdata;
  std::shared_ptr<portdata_dat_t> portdata;
  std::shared_ptr<upgrades_dat_t> upgrades;
  std::shared_ptr<techdata_dat_t> techdata;
  std::shared_ptr<mapdata_dat_t> mapdata;

  // kaitai parsed Tbl vectors
  std::vector<TblEntry> stat_txt_tbl_vec;
  std::vector<TblEntry> images_tbl_vec;
  std::vector<TblEntry> sfxdata_tbl_vec;
  std::vector<TblEntry> portdata_tbl_vec;
  std::vector<TblEntry> mapdata_tbl_vec;

private:
  // units.dat
  void init_units_dat();

  // orders.dat
  void init_orders_dat();

  // weapons.dat
  void init_weapons_dat();

  // flingy.dat
  void init_flingy_dat();

  // sprites.dat
  void init_sprites_dat();

  // images.dat
  void init_images_dat();

  // sfxdata.dat
  void init_sfxdata_dat();

  // portdata.dat
  void init_portdata_dat();

  // upgrades.dat
  void init_upgrades_dat();

  // techdata.dat
  void init_techdata_dat();

  // mapdata.dat
  void init_mapdata_dat();

  // stat_txt.tbl
  void init_stat_txt_tbl();

  // images.tbl
  void init_images_tbl();

  // sfxdata.tbl
  void init_sfxdata_tbl();

  // portdata.tbl
  void init_portdata_tbl();

  // mapdata.tbl
  void init_mapdata_tbl();

  std::shared_ptr<std::istream> m_units_stream;
  std::shared_ptr<std::istream> m_orders_stream;
  std::shared_ptr<std::istream> m_weapons_stream;
  std::shared_ptr<std::istream> m_flingy_stream;
  std::shared_ptr<std::istream> m_sprites_stream;
  std::shared_ptr<std::istream> m_images_stream;
  std::shared_ptr<std::istream> m_sfxdata_stream;
  std::shared_ptr<std::istream> m_portdata_stream;
  std::shared_ptr<std::istream> m_upgrades_stream;
  std::shared_ptr<std::istream> m_techdata_stream;
  std::shared_ptr<std::istream> m_mapdata_stream;
  std::shared_ptr<std::istream> m_iscript_stream;

  std::shared_ptr<kaitai::kstream> m_units_ks;
  std::shared_ptr<kaitai::kstream> m_orders_ks;
  std::shared_ptr<kaitai::kstream> m_weapons_ks;
  std::shared_ptr<kaitai::kstream> m_flingy_ks;
  std::shared_ptr<kaitai::kstream> m_sprites_ks;
  std::shared_ptr<kaitai::kstream> m_images_ks;
  std::shared_ptr<kaitai::kstream> m_sfxdata_ks;
  std::shared_ptr<kaitai::kstream> m_portdata_ks;
  std::shared_ptr<kaitai::kstream> m_upgrades_ks;
  std::shared_ptr<kaitai::kstream> m_techdata_ks;
  std::shared_ptr<kaitai::kstream> m_mapdata_ks;
  std::shared_ptr<kaitai::kstream> m_iscript_ks;

};

} /* namespace dat */

#endif /* DATAHUB_H */
