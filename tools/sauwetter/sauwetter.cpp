/*
 * sauwetter.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "Breeze.h"
#include "Storm.h"
#include "Casc.h"
#include "FileUtil.h"
#include "StringUtil.h"
#include "optparser.h"

// system
#include <iostream>

using namespace std;

// some global variables
string backend;
string archive;
string archive_file;
string destination_directory;

shared_ptr<Hurricane> selectChoosenBackend()
{
  shared_ptr<Hurricane> hurricane;

  cerr << "Backend: " << backend << endl;
  if(to_lower(backend) == "breeze")
  {
    hurricane = make_shared<Breeze>(archive);
  }
  else if(to_lower(backend) == "storm")
  {
    hurricane = make_shared<Storm>(archive);
  }
  else if(to_lower(backend) == "casc")
  {
#ifdef HAVE_CASC
    hurricane = make_shared<Casc>(archive);
#else
    cerr << "Error: No CASC support compiled into sauwetter!" << endl;
    exit(1);
#endif
  }

  return hurricane;
}

enum optionIndex
{
  UNKNOWN, HELP, BACKEND
};
const option::Descriptor usage[] =
{
  {
    UNKNOWN, 0, "", "", option::Arg::None, "USAGE: sauwetter [options] archive archive-file destination-directory\n\n"
    "Options:"
  },
  { HELP, 0, "h", "help", option::Arg::None, "  --help, -h  \t\tPrint usage and exit" },
  { BACKEND, 0, "b", "backend", Arg::Required, "  --backend BACKEND, -b BACKEND  \t\tChoose a backend (storm=St*arcr*ft1/Br**dwar;casc=Remastered;breeze=Folder)" },
  {
    UNKNOWN, 0, "", "", option::Arg::None,
    "\narchive \t\tDestination to the archive (mpq, casc or dummy folder) based on backend.\n"
    "\narchive-file \t\tThe file inside the archive (with relative path) that is to be extracted.\n"
    "\ndestination-directory \t\tWhere to save the extracted file with same relative path.\n"
  },
  { 0, 0, 0, 0, 0, 0 }
};

int parseOptions(int argc, const char **argv)
{
  argc -= (argc > 0);
  argv += (argc > 0); // skip program name argv[0] if present
  option::Stats stats(usage, argc, argv);
  std::unique_ptr<option::Option[]> options(new option::Option[stats.options_max]), buffer(new option::Option[stats.buffer_max]);
  option::Parser parse(usage, argc, argv, options.get(), buffer.get());

  if (parse.error())
    exit(0);

  if (options[HELP])
  {
    option::printUsage(std::cout, usage);
    exit(0);
  }

  if(options[BACKEND].count() > 0)
  {
    backend = options[BACKEND].arg;
  }
  else
  {
    cerr << "Error: 'backend' not given!" << endl << endl;
    option::printUsage(std::cout, usage);
    exit(1);
  }

  // parse options

  for (option::Option *opt = options[UNKNOWN]; opt; opt = opt->next())
    std::cout << "Unknown option: " << opt->name << "\n";

  for (int i = 0; i < parse.nonOptionsCount(); ++i)
  {
    switch (i)
    {
    case 0:
      //cerr << "archive #" << i << ": " << parse.nonOption(i) << "\n";
      archive = parse.nonOption(i);
      break;
    case 1:
      //cerr << "archive-file #" << i << ": " << parse.nonOption(i) << "\n";
      archive_file = parse.nonOption(i);
      break;
    case 2:
      //cerr << "destination-directory #" << i << ": " << parse.nonOption(i) << "\n";
      destination_directory = parse.nonOption(i);
      break;
    default:
      break;
    }
  }

  if (archive.empty())
  {
    cerr << "Error: 'archive' not given!" << endl << endl;
    option::printUsage(std::cout, usage);
    exit(1);
  }

  if (archive_file.empty())
  {
    cerr << "Error: 'archive_file' not given!" << endl << endl;
    option::printUsage(std::cout, usage);
    exit(1);
  }

  if (destination_directory.empty())
  {
    cerr << "Error: 'destination_directory' not given!" << endl << endl;
    option::printUsage(std::cout, usage);
    exit(1);
  }

  return 0;
}

int main(int argc, const char **argv)
{
  parseOptions(argc, argv);

  bool archive_exists = FileExists(archive);
  if(!archive_exists)
  {
    cerr << "archive not existing - exit!" << endl;
    exit(1);
  }

  shared_ptr<Hurricane> hurricane = selectChoosenBackend();

  string archive_file_slash(archive_file);
  replaceString("\\", "/", archive_file_slash);
  string destination_full_path = destination_directory + "/" + archive_file_slash;

  bool result = hurricane->extractFile(archive_file, destination_full_path, false);

  printf("extracted %s ... %s\n", destination_full_path.c_str(), result ? "ok" : "nok");

  return !result;
}

