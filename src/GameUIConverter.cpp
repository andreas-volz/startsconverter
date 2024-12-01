/*
 * GameUIConverter.cpp
 *
  *      Author: Andreas Volz
 */

/* project */
#include "GameUIConverter.h"
#include "pacman.h"
#include "Pcx.h"

/* system */
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;

GameUIConverter::GameUIConverter(std::shared_ptr<Hurricane> hurricane) :
  Converter(hurricane)
{
}

GameUIConverter::~GameUIConverter()
{
}

void GameUIConverter::convert(Storage gameUIStorage)
{
  std::ifstream json_file(pacman::searchFile("/dataset/gameui.json"));

  json gameui_json;

  json_file >> gameui_json;

  Pcx pcx(mHurricane);

  for(auto convert_entry : gameui_json)
  {
    pcx.load(convert_entry["mpq"]);
    pcx.savePNG(gameUIStorage(convert_entry["export"]));
  }
}
