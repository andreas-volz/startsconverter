/*
 * Bootstrap.h
 *
 *      Author: Andreas Volz
 */

/* project */
#include "Hurricane.h"
#include "Storage.h"

/* system */

#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

class Bootstrap
{
public:
  Bootstrap(const std::string &archive, const std::string &backend, Storage storage);
  virtual ~Bootstrap();

  std::shared_ptr<Hurricane> getMainArchive();
  std::shared_ptr<Hurricane> getSubArchive();


private:
  std::shared_ptr<Hurricane> mMainArchive;
  std::shared_ptr<Hurricane> mSubArchive;
  Storage mStorage;
};

#endif /* BOOTSTRAP_H */
