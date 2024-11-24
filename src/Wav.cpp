/*
 * Wav.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "Wav.h"
#include "Hurricane.h"
#include "platform.h"
#include "Logger.h"
#include "FileUtil.h"

// system
#include <vector>

using namespace std;

static Logger logger = Logger("startool.Wav");

Wav::Wav(std::shared_ptr<Hurricane> hurricane) :
  Converter(hurricane)
{
}

Wav::Wav(std::shared_ptr<Hurricane> hurricane, const std::string &arcfile) :
  Converter(hurricane)
{

}

Wav::~Wav()
{

}

bool Wav::convert(const std::string &arcfile,  Storage storage)
{
  bool result = true;

  string wav_file = storage.getFullPath() + ".wav";
  string ogg_file = storage.getFullPath() + ".ogg";

  result = mHurricane->extractFile(arcfile, wav_file, false);

  if(result)
  {
    string ffmpeg = "ffmpeg";
    vector<string> ffmped_args;
    ffmped_args.push_back("-y");

    ffmped_args.push_back("-i");
    ffmped_args.push_back(wav_file);

    ffmped_args.push_back("-acodec");
    ffmped_args.push_back("libvorbis");

    ffmped_args.push_back(ogg_file);

    string output_capture;
    int sys_call = platform::executeProcess(ffmpeg, ffmped_args, platform::OutputMode::CAPTURE, &output_capture);
    if(sys_call != 0)
    {
      result = false;
    }
    LOG4CXX_TRACE(logger, output_capture);

    fs::remove(wav_file);
  }

  return result;
}
