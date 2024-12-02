/*
 * CursorConverter.cpp
 *
 *      Author: Andreas Volz
 */

/* project */
#include "CursorConverter.h"
#include "Storage.h"
#include "pacman.h"
#include "Widgets.h"
#include "StringUtil.h"
#include "platform.h"

using namespace std;
using namespace nlohmann;

CursorConverter::CursorConverter(std::shared_ptr<Hurricane> hurricane, PaletteConverter &paletteConverter) :
  Converter(hurricane),
  mPaletteConverter(paletteConverter)
{
}

CursorConverter::~CursorConverter()
{
}

void CursorConverter::convert(Storage cursorStorage)
{
  std::ifstream cursorStream(pacman::searchFile("dataset/cursor.json"));
  json cursorArrayJson;
  cursorStream >> cursorArrayJson;

  Widgets widgets(mHurricane);
  widgets.setPalette(mPaletteConverter.getPalette("tunit"));

  for(auto cursorJson : cursorArrayJson)
  {
    string basename(to_lower(cutFileEnding(cursorJson)));
    replaceString("\\", "/", basename);
    fs::path basepath(basename);
    widgets.convert(cursorJson, cursorStorage(basepath.filename()));
  }

}

