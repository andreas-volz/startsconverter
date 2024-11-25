/*
 * ImagesConverter.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "ImagesConverter.h"
#include "Logger.h"
#include "Preferences.h"
#include "Storage.h"
#include "FileUtil.h"
#include "dat/Image.h"
#include "StringUtil.h"
#include "Grp.h"

// system
#include <iostream>
#include <fstream>
#include <string>


using namespace std;
using namespace dat;

static Logger logger = Logger("startool.ImagesConverter");

ImagesConverter::ImagesConverter(std::shared_ptr<Hurricane> hurricane, DataHub &datahub, PaletteManager &palette_manager) :
    Converter(hurricane),
    mDatahub(datahub),
    mPaletteManager(palette_manager)
{
}

ImagesConverter::~ImagesConverter()
{
}

void ImagesConverter::convert(Storage graphics)
{
  for (unsigned int i = 0; i < mDatahub.images->grp()->size(); i++)
  {
    Image image(mDatahub, i);

    string grp_name(image.grp_tbl().name1());
    grp_name = to_lower(grp_name); // make lower case to match it always

    LOG4CXX_TRACE(logger, "image: " + grp_name);

    /* The following code splits a full GRP path/file into a logic of image type, subtype and subsubtype.
     * The idea is to identify the logic which palette should be used to decode that specific GRP image.
     */

    string imageType;
    string imageSubType;
    string imageSubSubType;

    // find first slash
    size_t found = grp_name.find('\\');
    if(found != string::npos)
    {
      imageType = grp_name.substr (0, found);
      LOG4CXX_TRACE(logger, "imageType: " + imageType);

      // find second slash
      size_t found2 = grp_name.find('\\', found+1);
      if(found2 != string::npos)
      {
        imageSubType = grp_name.substr (found+1, found2 - found-1);
        LOG4CXX_TRACE(logger, "imageSubType: " + imageSubType);

        // find third slash
        size_t found3 = grp_name.find('\\', found2+1);
        if(found3 != string::npos)
        {
          imageSubSubType = grp_name.substr (found2+1, found3 - found2-1);
          LOG4CXX_TRACE(logger, "imageSubSubType: " + imageSubSubType);
        }
      }
    }

    string grp_arcfile =  "unit\\" + grp_name;

    Grp grp(mHurricane, grp_arcfile);
    std::shared_ptr<AbstractPalette> pal;
    string remapping;

    bool save_grp = true;

    if (image.draw_function() == images_dat_t::DRAW_FUNCTION_ENUM_REMAPPING)
    {
      if(image.remapping() == images_dat_t::REMAPPING_ENUM_OFIRE)
      {
        remapping = "ofire";
      }
      else if(image.remapping() == images_dat_t::REMAPPING_ENUM_GFIRE)
      {
        remapping = "gfire";
      }
      else if(image.remapping() == images_dat_t::REMAPPING_ENUM_BFIRE)
      {
        remapping = "bfire";
      }
      else if(image.remapping() == images_dat_t::REMAPPING_ENUM_BEXPL)
      {
        remapping = "bexpl";
      }
      else // as default use ofire until I've a better idea....
      {
        remapping = "ofire";
      }

      pal = mPaletteManager.getPalette(remapping);
      grp.setPalette(pal);

      grp.setRGBA(true);
    }
    else if (image.draw_function() == images_dat_t::DRAW_FUNCTION_ENUM_SHADOW)
    {
      // Hmm, this generates a black shadow, but I would expect alpha transparent shadow...
      save_grp = true;
      pal = mPaletteManager.getPalette("tfontgam");
      grp.setPalette(pal);
      grp.setRGBA(true);
    }
    else // all other drawing functions until I identify a special case
    {
      string tileset;

      if(imageType == "thingy" && imageSubType == "tileset" && !imageSubSubType.empty())
      {
        tileset = imageSubSubType;
        pal = mPaletteManager.getPalette(tileset);
      }
      else // in all other cases use the "tunit" palette
      {
        pal = mPaletteManager.getPalette("tunit");
      }
    }

    // FIXME: some blacklisting until I know why it crash!
    if(grp_name == "thingy\\blackx.grp")
    {
      save_grp = false;
    }
    // FIXME: some hard coded defaults to assign other palettes
    // make this configurable or find out if this is in the data
    else if(grp_name == "terran\\tank.grp")
    {
      // TODO: player color isn't available. But no problem visible for now.
      // maybe need to add cunit palette before into tileset palette?
      pal = mPaletteManager.getPalette("badlands");
    }
    else if(grp_name == "neutral\\cbattle.grp")
    {
      // TODO: player color isn't available. See how to fix this (or if this is needed for neutral)
      pal = mPaletteManager.getPalette("badlands");
    }
    else if(grp_name == "neutral\\ion.grp")
    {
      pal = mPaletteManager.getPalette("platform");
    }
    else if(grp_name == "neutral\\khyad01.grp")
    {
      pal = mPaletteManager.getPalette("jungle");
    }
    else if(grp_name == "neutral\\temple.grp")
    {
      pal = mPaletteManager.getPalette("jungle");
    }
    else if(grp_name == "neutral\\geyser.grp")
    {
      /* FIXME: only the first frame is correct, but this looks ok as we use only this one in animation
      frame 0 = "badlands.wpe" (default frame for all other tilesets)
      frame 1 = "platform.wpe"
      frame 2 = "install.wpe"
      frame 3 = "ashworld.wpe"
      */
      pal = mPaletteManager.getPalette("badlands");
    }
    grp.setPalette(pal);

    if(save_grp)
    {
      string grp_storage_file(grp_arcfile);
      replaceString("\\", "/", grp_storage_file);

      // cut the file ending and lower case it
      string grp_storage_file_base = to_lower(cutFileEnding(grp_storage_file, ".grp"));

      // if a remapping function is used for that Grp than save with specific name
      if(!remapping.empty())
      {
        grp_storage_file_base += "_" + remapping;
      }

      Storage png_file;
      png_file = graphics(grp_storage_file_base + ".png");

      grp.save(png_file);
    }
  }
}


