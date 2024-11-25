/*
 * FileUtil.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "FileUtil.h"
// system
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

bool FileExists(const std::string &filename)
{
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0 ? true : false;
}

void CheckPath(const std::string &path_name)
{
  try
  {
    fs::path p(path_name);
    fs::create_directories(p.parent_path());
  }
  catch (const std::exception& ex)
  {
    // just ignore the exception if not possible to create a directory
  }
}

