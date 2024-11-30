/* project */
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
#include "GraphicsConverter.h"
#include "PaletteConverter.h"
#include "SfxConverter.h"
#include "DatConverter.h"
#include "PortraitsConverter.h"
#include "TilesetConverter.h"
#include "CampaignConverter.h"
#include "iscript/IScript.h"
#include "iscript/IScriptConverter.h"
#include "FileUtil.h"

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
string converters;
vector<string> converters_list;

enum optionIndex
{
  UNKNOWN, HELP, BACKEND, CONVERTERS
};
const option::Descriptor usage[] =
{
  {
    UNKNOWN, 0, "", "", Arg::None, "USAGE: startsconverter [options] archive destination-directory\n\n"
    "Options:"
  },
  { HELP, 0, "h", "help", Arg::None, "  --help, -h  \t\tPrint usage and exit" },
  { BACKEND, 0, "b", "backend", Arg::Required, "  --backend=<backend>, -b <backend>  \t\tChoose a backend (storm=St*arcr*ft1/Br**dwar;casc=Remastered;breeze=Folder)" },
  { CONVERTERS, 0, "c", "converters", Arg::Required, "  --converters=<conv1:conv2:conv3>, -c <conv1:conv2:conv3>   \t\tUnordered list of converters to run. If not used run all converters." },
  {
    UNKNOWN, 0, "", "", Arg::None,
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

  if(options[CONVERTERS].count() > 0)
  {
    converters = options[CONVERTERS].arg;
    converters_list = splitString(converters, ":");
  }
  else
  {
    converters = "all";
    converters_list.push_back("all");
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

template <typename... Strings>
bool converterCheck(const Strings&... strings)
{
    return (... || (std::find(converters_list.begin(), converters_list.end(), strings) != converters_list.end()));
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

  cout << "List of choosen converters: ";
  printVector<string>(converters_list);
  // TODO: check for allowed converter options and print them as help

  Storage archiveStorage;
  archiveStorage.setDataPath(destination_directory);

  CheckPath(destination_directory);

  Bootstrap bootstrap(archive, backend, archiveStorage);

  // this is needed everywhere so create it without options
  dat::DataHub datahub(bootstrap.getSubArchive());

  // needs to stay outside because we need it later
  PaletteConverter palette_converter(bootstrap.getSubArchive());
  if(converterCheck("all", "palette", "graphics", "tileset", "campaign"))
  {
    Storage paletteStorage;
    paletteStorage.setDataPath(destination_directory);
    paletteStorage.setDataType("palette");

    cout << "Run PaletteManager...";fflush(stdout);
    palette_converter.convert(paletteStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "graphics"))
  {
    Storage graphicsStorage;
    graphicsStorage.setDataPath(destination_directory);
    graphicsStorage.setDataType("graphics");

    cout << "Run GraphicsConverter...";fflush(stdout);
    GraphicsConverter graphics_converter(bootstrap.getSubArchive(), datahub, palette_converter);
    graphics_converter.convert(graphicsStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "sfx"))
  {
    Storage soundsStorage;
    soundsStorage.setDataPath(destination_directory);
    soundsStorage.setDataType("sounds");

    cout << "Run SfxConverter...";fflush(stdout);
    SfxConverter sfx_converter(bootstrap.getSubArchive(), datahub);
    sfx_converter.convert(soundsStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "dat"))
  {
    Storage datStorage;
    datStorage.setDataPath(destination_directory);

    cout << "Run DatConverter...";fflush(stdout);
    DatConverter dat_converter(bootstrap.getSubArchive(), datahub);
    dat_converter.convert(datStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "portraits"))
  {
    Storage portraitsStorage;
    portraitsStorage.setDataPath(destination_directory);
    portraitsStorage.setDataType("portraits");

    cout << "Run PortraitsConverter...";fflush(stdout);
    PortraitsConverter portraits_converter(bootstrap.getSubArchive(), datahub);
    portraits_converter.convert(portraitsStorage);
    cout << "DONE" << endl;
  }

  // needs to stay outside because we need it later
  TilesetConverter tileset_converter(bootstrap.getSubArchive(), palette_converter);
  if(converterCheck("all", "tileset", "campaign")) // TODO: could optimize "tileset->campaign" dependency, see comments inside
  {
    Storage tilesetStorage;
    tilesetStorage.setDataPath(destination_directory);
    tilesetStorage.setDataType("tileset");

    cout << "Run TilesetConverter...";fflush(stdout);
    tileset_converter.convert(tilesetStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "campaign"))
  {
    Storage campaignStorage;
    campaignStorage.setDataPath(destination_directory);
    campaignStorage.setDataType("campaign");

    cout << "Run CampaignConverter...";fflush(stdout);
    CampaignConverter campaign_converter(bootstrap.getMainArchive(), datahub, tileset_converter);
    campaign_converter.convert(campaignStorage);
    cout << "DONE" << endl;
  }

  if(converterCheck("all", "iscript"))
  {
    // TODO: support Strorage
    // TODO: redesign Iscript to only call the Converter and wrap the IScript class
    string iscript_bin = "scripts\\iscript.bin";
    IScript iscript(bootstrap.getSubArchive(), iscript_bin);
    iscript.parseIScript();

    std::vector<Opcode> &opcode_vector = iscript.getOpcodeVector();
    std::map<uint16_t, std::vector<uint16_t>> &iscript_scpe_header_map = iscript.getIScriptSCPEHeaderMap();

    IScriptConverter iscript_converter;
    iscript_converter.setHumanReadable(false);
    string iscript_txt = destination_directory + "/iscript/iscript.txt";
    CheckPath(iscript_txt); // TODO: this should be inside the Converter
    cout << "Run IScriptConverter...";fflush(stdout);
    iscript_converter.saveConverted(iscript_txt, iscript_scpe_header_map, opcode_vector);
    cout << "DONE" << endl;
  }

  cout << "App Finished" << endl;
  return 0;
}
