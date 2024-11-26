/* project */
#include <GraphicsConverter.h>
#include "Breeze.h"
#include "Storm.h"
#include "Casc.h"
#include "FileUtil.h"
#include "Storage.h"
#include "optparser.h"
#include "Logger.h"
#include "StringUtil.h"
#include "Bootstrap.h"
#include "dat/DataHub.h"
#include "PaletteManager.h"
#include "SfxConverter.h"
#include "DatConverter.h"
#include "PortraitsConverter.h"

/* system */
#include <string>
#include <stdexcept>

using namespace std;

static Logger logger("startool.startsconverter");

// some global variables
bool pretty = true;
string backend;
string archive;
string archive_file;
string destination_directory;

enum optionIndex
{
  UNKNOWN, HELP, PRETTY, BACKEND
};
const option::Descriptor usage[] =
{
  {
    UNKNOWN, 0, "", "", option::Arg::None, "USAGE: startsconverter [options] archive destination-directory\n\n"
    "Options:"
  },
  { HELP, 0, "h", "help", option::Arg::None, "  --help, -h  \t\tPrint usage and exit" },
  { PRETTY, 0, "p", "pretty", Arg::Required, "  --pretty=[yes/no], -p  \t\tPretty print the formated JSON file (default: yes)" },
  { BACKEND, 0, "b", "backend", Arg::Required, "  --backend, -b  \t\tChoose a backend (storm=St*arcr*ft1/Br**dwar;casc=Remastered;breeze=Folder)" },
  { HELP, 0, "h", "help", option::Arg::None, "  --help, -h  \t\tPrint usage and exit" },
  {
    UNKNOWN, 0, "", "", option::Arg::None,
    "\narchive \t\tDestination to the archive (mpq, casc or dummy folder) based on backend.\n"
    "\ndestination-directory \t\tWhere to save the extracted file with same relative path.\n\n"
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

  if ( options[PRETTY].count() > 0 )
  {
    if (string(options[PRETTY].arg) == "yes")
    {
      pretty = true;
    }
    else if (string(options[PRETTY].arg) == "no")
    {
      pretty = false;
    }
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
      archive = parse.nonOption(i);
      break;
    case 1:
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
#ifdef HAVE_LOG4CXX
  if(FileExists("logging.prop"))
  {
    log4cxx::PropertyConfigurator::configure("logging.prop");
  }
  else
  {
    logger.off();
  }
#endif // HAVE_LOG4CXX

  parseOptions(argc, argv);

  Storage archiveStorage;
  archiveStorage.setDataPath(destination_directory);

  Storage paletteStorage;
  paletteStorage.setDataPath(destination_directory);
  paletteStorage.setDataType("palette");

  Storage graphicsStorage;
  graphicsStorage.setDataPath(destination_directory);
  graphicsStorage.setDataType("graphics");

  Storage soundsStorage;
  soundsStorage.setDataPath(destination_directory);
  soundsStorage.setDataType("sounds");

  Storage datStorage;
  datStorage.setDataPath(destination_directory);

  Storage portraitsStorage;
  portraitsStorage.setDataPath(destination_directory);
  portraitsStorage.setDataType("portraits");

  CheckPath(destination_directory);

  Bootstrap bootstrap(archive, backend, archiveStorage);

  dat::DataHub datahub(bootstrap.getSubArchive());

  //PaletteManager palette_manager(bootstrap.getSubArchive());
  //palette_manager.convert(paletteStorage);

  //GraphicsConverter graphics_converter(bootstrap.getSubArchive(), datahub, palette_manager);
  //graphics_converter.convert(graphicsStorage);

  //SfxConverter sfx_converter(bootstrap.getSubArchive(), datahub);
  //sfx_converter.convert(soundsStorage);

  //DatConverter dat_converter(bootstrap.getSubArchive(), datahub);
  //dat_converter.convert(datStorage);

  //PortraitsConverter portraits_converter(bootstrap.getSubArchive(), datahub);
  //portraits_converter.convert(portraitsStorage);




  cout << "App Finished" << endl;
  return 0;
}
