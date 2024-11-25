/*
 * Bootstrap.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "Bootstrap.h"
#include "StringUtil.h"
#include "Breeze.h"
#include "Storm.h"
#include "Casc.h"
#include "platform.h"

/* system */
#include <iostream>
#include <memory>

using namespace std;

Bootstrap::Bootstrap(const std::string &archive, const std::string &backend, Storage storage) :
  mStorage(storage)
{
  if(to_lower(backend) == "breeze")
  {
    mMainArchive = make_shared<Breeze>(archive);
  }
  else if(to_lower(backend) == "storm")
  {
    mMainArchive = make_shared<Storm>(archive);
  }
  else if(to_lower(backend) == "casc")
  {
#ifdef HAVE_CASC
    mMainArchive = make_shared<Casc>(archive);
#else
    cerr << "Error: No CASC support compiled into Application!" << endl;
    exit(1);
#endif
  }
  else
  {
    cerr << "Error: Unknown backend: " << backend << endl;
  }

  if(mMainArchive)
  {
    string sub_mpq_name = "stardat.mpq";
    bool extracted = mMainArchive->extractFile("files\\stardat.mpq", storage(sub_mpq_name));
    if(!extracted)
    {
      sub_mpq_name = "broodat.mpq";
      extracted = mMainArchive->extractFile("files\\broodat.mpq", storage(sub_mpq_name));
    }
    if(extracted)
    {
      mSubArchive = make_shared<Storm>(storage(sub_mpq_name));
    }
  }
}

Bootstrap::~Bootstrap()
{
  // clean up both temporary sub mpq files after unpacking all files
  fs::remove(string(mStorage("stardat.mpq")));
  fs::remove(string(mStorage("broodat.mpq")));
}

std::shared_ptr<Hurricane> Bootstrap::getMainArchive()
{
  return mMainArchive;
}
std::shared_ptr<Hurricane> Bootstrap::getSubArchive()
{
  return mSubArchive;
}
