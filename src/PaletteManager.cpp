/*
 * PaletteManager.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "PaletteManager.h"
#include "Hurricane.h"
#include "Breeze.h"
#include "pacman.h"
#include "Palette.h"
#include "FileUtil.h"
#include "Pcx.h"
#include "StringUtil.h"

/* system */
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <memory>

using namespace std;
using json = nlohmann::json;

PaletteManager::PaletteManager(std::shared_ptr<Hurricane> hurricane) :
  Converter(hurricane)
{
}

PaletteManager::~PaletteManager()
{
}

std::shared_ptr<AbstractPalette> PaletteManager::getPalette(const std::string palette_name)
{
  return mPaletteMap.at(palette_name);
}

void PaletteManager::convert(Storage palStorage)
{
  // read in the json file
  std::ifstream json_file(pacman::searchFile("/dataset/palettes.json"));

  json palettes_json; //create unitiialized json object

  json_file >> palettes_json; // initialize json object with what was read from file

  //std::cout << units_json << std::endl; // prints json object to screen

  vector<string> wpeNames;

  /** WPE **/
  auto &wpe_section = palettes_json.at("WPE");

  for(auto &wpe_array : wpe_section)
  {
    string name = wpe_array.at("name");
    string wpe_arcfile = wpe_array.at("arcfile");
    string pal_palette = wpe_array.at("palette");

    shared_ptr<DataChunk> dataWPE = mHurricane->extractDataChunk(wpe_arcfile);

    if (dataWPE) // load from WPE palette
    {
      shared_ptr<Palette> pal = make_shared<Palette>(dataWPE);
      mPaletteMap[name] = pal;
      wpeNames.push_back(name);

      string pal_file(palStorage.getFullPath() + pal_palette);
      CheckPath(pal_file);
      pal->write(pal_file);
    }
    else // load from stored .pal palette
    {
      Breeze localPal(palStorage.getFullPath());
      shared_ptr<DataChunk> dataPal = localPal.extractDataChunk(pal_palette);

      if(dataPal)
      {
        std::shared_ptr<Palette> pal = make_shared<Palette>(dataPal);
        mPaletteMap[name] = pal;
      }
    }

    //cout << wpe_array << endl;
  }

  /** PCX **/
  auto &pcx_section = palettes_json.at("PCX");

  for(auto &pcx_array : pcx_section)
  {
    string name = pcx_array.at("name");
    string pcx_arcfile = pcx_array.at("arcfile");
    string pal_palette = pcx_array.at("palette");

    Pcx pcx(mHurricane, pcx_arcfile);

    if(!pcx.getSize().isEmpty()) // load from PCX palette
    {
      std::shared_ptr<Palette> pal;
      try
      {
        auto &pcx_mapping = pcx_array.at("mapping");

        int length = pcx_mapping.at("length");
        int start = pcx_mapping.at("start");
        int index = pcx_mapping.at("index");

        pal = pcx.mapIndexPalette(length, start, index);
      }
      catch (const nlohmann::detail::out_of_range &json_range)
      {
        pal = pcx.getPalette();
      }

      string pal_file(palStorage.getFullPath() + pal_palette);
      CheckPath(pal_file);
      pal->write(pal_file);

      mPaletteMap[name] = pal;
    }
    else // load from stored .pal palette
    {
      Breeze localPal(palStorage.getFullPath());
      shared_ptr<DataChunk> dataPal = localPal.extractDataChunk(pal_palette);

      if(dataPal)
      {
        std::shared_ptr<Palette> pal = make_shared<Palette>(dataPal);
        mPaletteMap[name] = pal;
      }
    }

    //cout << pcx_array << endl;
  }

  /** PCX2D (after WPE block has been read in) **/
  auto &pcx2d_section = palettes_json.at("PCX2D");

  for(auto &pcx_array : pcx2d_section)
  {
    string pcx_name = pcx_array.at("name");
    string pcx_arcfile = pcx_array.at("arcfile");
    string pal_palette = pcx_array.at("palette");

    // replace this with the first of the WPE palettes. Which one doesn't care for the palette logic.
    replaceString("<?>", *wpeNames.begin(), pcx_arcfile);

    Pcx pcx(mHurricane, pcx_arcfile);

    std::shared_ptr<Palette2D> pal2D = pcx.map2DPalette();

    string pal_file(palStorage.getFullPath() + pal_palette);
    CheckPath(pal_file);
    pal2D->write(pal_file);

    mPaletteMap[pcx_name] = pal2D;

    //cout << pcx_array << endl;
  }


}
