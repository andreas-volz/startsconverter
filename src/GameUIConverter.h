/*
 * GameUIConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef GAMEUICONVERTER_H
#define GAMEUICONVERTER_H

/* project */
#include "Converter.h"
#include "Storage.h"

class GameUIConverter : public Converter
{
public:
  GameUIConverter(std::shared_ptr<Hurricane> hurricane);
  virtual ~GameUIConverter();

  void convert(Storage gameUIStorage);
};

#endif /* GAMEUICONVERTER_H */
