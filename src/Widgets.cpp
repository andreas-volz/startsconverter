/*
 * Widget.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include <Palette.h>
#include <PngExporter.h>
#include "Widgets.h"
#include "Storm.h"
#include "FileUtil.h"
#include "pacman.h"

// system
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

Widgets::Widgets(std::shared_ptr<Hurricane> hurricane) :
  Grp(hurricane)
{
}

Widgets::~Widgets()
{
}

bool Widgets::convert(const std::string &arcfile, Storage filename, bool frameStitching, const std::string &name)
{
  bool result = true;

  std::shared_ptr<DataChunk> dcGrp = mHurricane->extractDataChunk(arcfile);

  std::vector<char> GrpVec = dcGrp->getCharVector();

  mGRPImage.LoadImage(&GrpVec, true); // true: no duplicate widgets needed

  CheckPath(filename.getFullPath());

  if(dcGrp)
  {
    if(frameStitching == false)
    {
      string image_basename(filename.getFullPath() + "/" + name);
      mGRPImage.SaveSinglePNG(image_basename + "%d.png", 0, mGRPImage.getNumberOfFrames(), true, false); // no best fit, so create frames of same size

      json j_frames_info;
      for(unsigned int i = 0; i < mGRPImage.getNumberOfFrames(); i++)
      {
        json j_frame_info = name + to_string(i) + ".png";
        j_frames_info.push_back(j_frame_info);
      }

      // save a JSON with all frames in the correct order
      string json_path(filename.getFullPath() + "/" + filename.getFilename() + ".json");
      saveJson(j_frames_info, json_path, true);
    }
    else
    {
      string image_basename(filename.getFullPath());
      mGRPImage.SaveStitchedPNG(image_basename + ".png", 0, mGRPImage.getNumberOfFrames(), 0, true);

      // save json file beside each png file with tilesize information in it. The engine needs this information.
      string json_path(filename.getFullPath() + ".json");
      json j_grp_info = json::object({ {"width", mGRPImage.getMaxImageWidth()}, {"height", mGRPImage.getMaxImageHeight()} });
      saveJson(j_grp_info, json_path, true);
    }
  }

  return result;
}

bool Widgets::convert(const std::string &arcfile, Storage targetpath, json &frameExtractJson)
{
  bool result = true;

  std::shared_ptr<DataChunk> dcGrp = mHurricane->extractDataChunk(arcfile);

  std::vector<char> GrpVec = dcGrp->getCharVector();

  mGRPImage.LoadImage(&GrpVec, true); // true: no duplicate widgets needed

  CheckPath(targetpath.getFullPath());

  if(dcGrp)
  {
    for(auto &array : frameExtractJson)
    {
      string name = array.at("name");
      nlohmann::json frameArray = array.at("frame");

      int frameStitching = 0;
      vector<int> stitchedFrames;
      for(auto frame : frameArray)
      {
        //cout << "frame: " << frame << endl;
        stitchedFrames.push_back(frame);
        frameStitching++;
      }

      if(frameStitching == 1)
      {
        //frameSingleNames.push_back(name);
        mGRPImage.SaveSinglePNG(targetpath.getFullPath() + "/" + name, *stitchedFrames.begin(), *stitchedFrames.begin()+1, true);
      }
      else if(frameStitching > 1)
      {
        string image_basename(targetpath.getFullPath() + "/" + name);
        mGRPImage.SaveStitchedPNG(image_basename, stitchedFrames, 0, true);
      }
      else
      {
        cerr << "something wrong with frame array!" << endl;
      }
    }
  }

  return result;
}

