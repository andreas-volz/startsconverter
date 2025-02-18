/*
 * Converter.h
 *
 *      Author: Andreas Volz
 */

#ifndef CONVERTER_H
#define CONVERTER_H

// System
#include <memory>
#include <string>

// Forward declarations
class Hurricane;

class Converter
{
public:
  Converter(std::shared_ptr<Hurricane> hurricane);
  virtual ~Converter();

protected:
  std::shared_ptr<Hurricane> mHurricane;
};

#endif /* CONVERTER_H */
