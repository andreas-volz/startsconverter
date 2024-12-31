/*
 * Order.h
 *
 *      Author: Andreas Volz
 */

#ifndef ORDER_H
#define ORDER_H

// project
#include "ObjectAccess.h"
#include "Tech.h"
#include "Weapon.h"

namespace dat
{

class Order : public ObjectAccess
{
public:
  Order(DataHub &datahub, unsigned int id);
  virtual ~Order();

  uint16_t label();
  TblEntry label_tbl();

  bool use_weapon_targeting();

  uint8_t unknown2();

  uint8_t unknown3();

  uint8_t unknown4();

  uint8_t unknown5();

  bool interruptible();

  uint8_t unknown7();

  bool queueable();

  uint8_t unknown9();

  uint8_t unknown10();

  uint8_t unknown11();

  uint8_t unknown12();

  uint8_t targeting();
  Weapon targeting_obj();

  uint8_t energy();
  Tech energy_obj();

  uint8_t animation();

  uint16_t highlight();

  uint16_t unknown17();

  uint8_t obscured_order();

  // TODO: the recursive object return logic doesn't work here as this ends in a endless loop
  //Order obscured_order_obj();

};

} /* namespace dat */

#endif /* ORDER_H */
