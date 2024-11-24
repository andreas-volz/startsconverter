/*
 * Video.h
 *
 *      Author: Andreas Volz
 */

#ifndef VIDEO_H
#define VIDEO_H

// project
#include "Storage.h"
#include "Converter.h"

// System
#include <memory>

class Smacker : public Converter
{
public:
  Smacker(std::shared_ptr<Hurricane> hurricane);
  virtual ~Smacker();

  /**
   *  Convert SMK video to OGV
   */
  bool convert(const std::string &arcfile, Storage storage);

private:

};

#endif /* VIDEO_H */
