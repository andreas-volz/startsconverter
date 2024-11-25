/*
 * PortraitsConverter.h
 *
 *      Author: Andreas Volz
 */

#ifndef PORTRAITSCONVERTER_H
#define PORTRAITSCONVERTER_H

/* project */
#include "dat/DataHub.h"
#include "Storage.h"

/* system */

class PortraitsConverter : public Converter
{
public:
  PortraitsConverter(std::shared_ptr<Hurricane> hurricane, dat::DataHub &datahub);
  virtual ~PortraitsConverter();

  bool convert(Storage videosStorage);

private:
  /**
   * check all smk files for a portrait, export them, convert to mng and save the raw name
   * @param portrait_list The function fills this vector to later export to LUA
   */
  bool convertOgvPortraits(const std::string &arcfile, std::vector<std::string> &portrait_list, Storage videosStorage);


  // if used more often put into a utils namespace...
  /*template<typename T>
  void removeDuplicates(std::vector<T>& vec)
  {
      std::sort(vec.begin(), vec.end());
      vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }*/

  dat::DataHub &mDatahub;
};

#endif /* PORTRAITSCONVERTER_H */
