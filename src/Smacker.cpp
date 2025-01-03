/*
 * Video.cpp
 *
 *      Author: Andreas Volz
 */

// Local
#include <Smacker.h>
#include "Hurricane.h"
#include "FileUtil.h"
#include "platform.h"
#include "Logger.h"

// System
#include <iostream>
#include <vector>

using namespace std;

static Logger logger = Logger("startool.Smacker");

/*
 * TODO: do this for portraits: check if size < 64 and not quadratic and then add UI border around
  Until I find a solution I'll workaround by padding some pixels and put a UI frame in front:
  ffmpeg -y -i tscfid02.smk -vf "pad=width=64:height=64:x=2:y=4:color=black" -codec:v libtheora -codec:a libvorbis -pix_fmt yuv420p tscfid02.ogv
*/

Smacker::Smacker(std::shared_ptr<Hurricane> hurricane) :
  Converter(hurricane)
{

}

Smacker::~Smacker()
{

}

bool Smacker::convert(const std::string &arcfile,  Storage storage)
{
  bool result = true;

  string smk_file = storage.getFullPath() + ".smk";
  string ogv_file = storage.getFullPath() + ".ogv";

  result = mHurricane->extractFile(arcfile, smk_file, false);
  if(result)
  {
    string ffmpeg = "ffmpeg";
    vector<string> ffmped_args;
    ffmped_args.push_back("-y");

    ffmped_args.push_back("-i");
    ffmped_args.push_back(smk_file);

    ffmped_args.push_back("-codec:v");
    ffmped_args.push_back("libtheora");

    ffmped_args.push_back("-qscale:v");
    ffmped_args.push_back("31");

    ffmped_args.push_back("-codec:a");
    ffmped_args.push_back("libvorbis");

    ffmped_args.push_back("-qscale:a");
    ffmped_args.push_back("15");

    ffmped_args.push_back("-pix_fmt");
    ffmped_args.push_back("yuv420p");

    ffmped_args.push_back(ogv_file);

    string output_capture;
    int sys_call = platform::executeProcess(ffmpeg, ffmped_args, platform::OutputMode::CAPTURE, &output_capture);
    if(sys_call != 0)
    {
      result = false;
    }
    LOG4CXX_TRACE(logger, output_capture);

    fs::remove(smk_file);
  }

  return result;
}

