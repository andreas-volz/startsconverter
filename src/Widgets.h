/*
 * Widget.h
 *
 *      Author: Andreas Volz
 */

#ifndef WIDGETS_H_
#define WIDGETS_H_

/* project */
#include "Grp.h"
#include "Palette.h"

// system
#include <nlohmann/json.hpp>

class Widgets: public Grp
{
public:
  Widgets(std::shared_ptr<Hurricane> hurricane);
  virtual ~Widgets();

  /**
   * Convert a widget from data container as several PNG files.
   *
   * @param arcfile The archive path inside the archive file
   * @param targetpath A path where all the generated PNG files are saved
   * @param frameStitching decide to save all frames in one stitched image or several single ones
   * @param name The prefix name for the generated frames (default: frame.png for stitched version, frame%d.png for single version)
   */
  bool convert(const std::string &arcfile, Storage filename,  bool frameStitching = false, const std::string &name = "frame");

  /**
   * Convert a widget from data container as several PNG files with detail control about the frame names.
   *
   * @param arcfile The archive path inside the archive file
   * @param targetpath A path where all the generated PNG files are saved
   * @param frameExtractJson This specific JSON file controls each frames name and also which frames should be stitched together
   */
  bool convert(const std::string &arcfile, Storage targetpath, nlohmann::json &frameExtractJson);

private:

};

#endif /* WIDGETS_H_ */
