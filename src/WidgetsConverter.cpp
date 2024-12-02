/*
 * WidgetsConverter.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "WidgetsConverter.h"
#include "Widgets.h"
#include "pacman.h"

/* system */
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

WidgetsConverter::WidgetsConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter) :
    Converter(hurricane),
    mPaletteConverter(paletteConverter)
{

}

WidgetsConverter::~WidgetsConverter()
{

}

void WidgetsConverter::convert(Storage widgetStorage)
{
  std::ifstream dlgsRaceJsonStream(pacman::searchFile("dataset/dlgs_race.json"));
  json dlgsRaceJson; //create unitiialized json object
  dlgsRaceJsonStream >> dlgsRaceJson; // initialize json object with what was read from file

  /**
   * just hard coded those race GRPs. I don't think an additional JSON file around will help anyone...
   */

  Widgets widgets(mHurricane);
  widgets.setPalette(mPaletteConverter.getPalette("tunit"));
  widgets.convert("dlgs\\terran.grp", widgetStorage("dlgs/terran"), dlgsRaceJson);
  widgets.convert("dlgs\\zerg.grp", widgetStorage("dlgs/zerg"), dlgsRaceJson);
  widgets.convert("dlgs\\protoss.grp", widgetStorage("dlgs/protoss"), dlgsRaceJson);

}
