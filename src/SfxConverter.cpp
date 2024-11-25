/*
 * SfxConverter.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "SfxConverter.h"
#include "Storage.h"
#include "FileUtil.h"
#include "dat/Sfx.h"
#include "Wav.h"
#include "Logger.h"
#include "StringUtil.h"

// system
#include <iostream>
#include <fstream>

using namespace std;
using namespace dat;

static Logger logger = Logger("startool.SfxConverter");


SfxConverter::SfxConverter(std::shared_ptr<Hurricane> hurricane, DataHub &datahub) :
  Converter(hurricane),
  mDatahub(datahub)
{
}

SfxConverter::~SfxConverter()
{
}

void SfxConverter::convert(Storage sounds)
{
  // start with i=1 as 0=none and couldn't be read
  for(unsigned int i = 1; i < static_cast<unsigned int>(mDatahub.sfxdata->num_lines()); i++)
  {
    Sfx sfx(mDatahub, i);

    string sound_id;

    TblEntry sound_file = sfx.sound_file_tbl();

    string sound_arcfile("sound\\" + sound_file.name1());
    string sound_file_base(sound_file.name1());
    replaceString("\\", "/", sound_file_base);
    sound_file_base = cutFileEnding(to_lower(sound_file_base), ".wav");

    LOG4CXX_TRACE(logger, "sound_arcfile: " + sound_arcfile);


    Wav wav(mHurricane);

    bool wav_result = wav.convert(sound_arcfile, sounds(sound_file_base));
    LOG4CXX_TRACE(logger, "save Sfx file: " + sound_file_base + ": " + to_string(wav_result));
  }
}
