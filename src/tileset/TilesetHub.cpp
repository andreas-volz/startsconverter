/*
 * TilesetHub.cpp
 *
 *      Author: Andreas Volz
 */

// project
#include "TilesetHub.h"
#include "PaletteImage.h"
#include "PngExporter.h"
#include "MegaTile.h"
#include "MiniTile.h"
#include "FileUtil.h"
#include "Preferences.h"
#include "Hurricane.h"
#include "FileUtil.h"

// system
#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

namespace tileset
{

const Size TilesetHub::MEGATILE_SIZE = Size(32, 32);
const Size TilesetHub::MINITILE_SIZE = Size(8, 8);

TilesetHub::TilesetHub(std::shared_ptr<Hurricane> hurricane, const std::string &tilesetName) :
  Converter(hurricane),
  mTilesetName(tilesetName)
{
  init();
}

TilesetHub::~TilesetHub()
{

}

void TilesetHub::init()
{
  string fullTilesetName = "tileset\\" + mTilesetName;

  m_cv5_stream = mHurricane->extractStream(fullTilesetName + ".cv5");
  m_cv5_ks = make_shared<kaitai::kstream>(&*m_cv5_stream);
  cv5 = make_shared<tileset_cv5_t>(m_cv5_ks.get());

  m_vx4_stream = mHurricane->extractStream(fullTilesetName + ".vx4");
  m_vx4_ks = make_shared<kaitai::kstream>(&*m_vx4_stream);
  vx4 = make_shared<tileset_vx4_t>(m_vx4_ks.get());

  m_vf4_stream = mHurricane->extractStream(fullTilesetName + ".vf4");
  m_vf4_ks = make_shared<kaitai::kstream>(&*m_vf4_stream);
  vf4 = make_shared<tileset_vf4_t>(m_vf4_ks.get());

  m_vr4_stream = mHurricane->extractStream(fullTilesetName + ".vr4");
  m_vr4_ks = make_shared<kaitai::kstream>(&*m_vr4_stream);
  vr4 = make_shared<tileset_vr4_t>(m_vr4_ks.get());
}

bool TilesetHub::convertTiledFormat(std::shared_ptr<AbstractPalette> palette, Storage storage)
{
  if(!vx4) // if it isn't available just return false
  {
    return false;
  }

  if(!palette) // if something wrong with palette just return false
  {
    return false;
  }

  unsigned int num_tiles = vx4->array()->size();
  int tiles_width = MEGATILE_COLUMNS;
  int tiles_height = static_cast<int>(ceil(static_cast<float>(num_tiles) / static_cast<float>(tiles_width)));
  Size ultra_tile_size = Size(tiles_width, tiles_height);

  /*
   * identify how many tiles have palette animation to calculate maximum image size
   */
  for(unsigned int i = 0; i < num_tiles; i++)
  {
    MegaTile mega(*this, i);

    std::shared_ptr<PaletteImage> palette_image = mega.getPaletteImage();
    /*
     * TODO: Those ranges are only estimates by experiments for water and lava. I need more research.
     * Could be slightly other pixels and more ranges. I'm even not sure if all tilesets have the same range.
     * I need to put this somewhere in the json file. Maybe palettes.json. A correct solution needs then to use the same pixels
     * later below in createShiftVector(createRangeVector())
     */
    bool index_found = palette_image->hasPaletteIndexRange(1, 6) || palette_image->hasPaletteIndexRange(7, 13);
    if(index_found)
    {
      animation_tiles.push_back(i);
    }
  }

  Size ultra_animation_tile_size = Size(TILE_ANIMATION_FRAMES, animation_tiles.size());

  TiledPaletteImage ultraTile(ultra_tile_size, MEGATILE_SIZE);
  TiledPaletteImage ultraTileAnimation(ultra_animation_tile_size, MEGATILE_SIZE);

  unsigned int anim_group = 0;
  for(unsigned int i = 0; i < num_tiles; i++)
  {
    MegaTile mega(*this, i);

    std::shared_ptr<PaletteImage> palette_image = mega.getPaletteImage();
    auto found_it = find(animation_tiles.begin(), animation_tiles.end(), i);
    if(found_it != animation_tiles.end())
    {
      for(unsigned int frame = 0; frame < TILE_ANIMATION_FRAMES; frame++)
      {
        auto replacer = PaletteImage::createWrapVector(PaletteImage::createRangeVector(1, 6), frame);
        auto replacer2 = PaletteImage::createWrapVector(PaletteImage::createRangeVector(7, 13), frame);
        replacer.insert(replacer.end(), replacer2.begin(), replacer2.end());

        PaletteImage replaced_image(*palette_image, replacer);
        ultraTileAnimation.copyTile(replaced_image, Pos(frame, anim_group));
      }

      anim_group++;
    }
    else
    {
      // TODO: save animated tiles in static tileset and remember index, then no need to export the static tile (maybe)
    }

    max_static_tiles++;
    ultraTile.copyTile(*palette_image, i);
  }

  storage.setFilename("tiled/" + mTilesetName);
  string save_png(storage.getFullPath() + ".png");
  string save_png_anim(storage.getFullPath() + "_animation.png");
  CheckPath(save_png);
  CheckPath(save_png_anim);
  PngExporter::save(save_png, ultraTile, palette, false, false);
  generateTilesetJson(storage);

  /*
   * don't export animations if none found
   */
  if(animation_tiles.size() > 0)
  {
    PngExporter::save(save_png_anim, ultraTileAnimation, palette, false, false);
    generateAnimationTilesetJson(anim_group, storage);
  }

  return true; // hack
}

void TilesetHub::generateVR4MiniTilePng(std::shared_ptr<AbstractPalette> palette, Storage storage)
{
  unsigned int num_tiles = vr4->array()->size();
  Size ultra_tile_size;
  ultra_tile_size.setWidth(sqrt(num_tiles));
  ultra_tile_size.setHeight(num_tiles / ultra_tile_size.getWidth() + 1);

  TiledPaletteImage ultraTile(ultra_tile_size, MINITILE_SIZE);

  for(unsigned int i = 0; i < num_tiles; i++)
  {
    MiniTile mini(*this, i);

    std::shared_ptr<PaletteImage> palette_image = mini.getPaletteImage();

    // TODO: here copy of animation tiles...

    ultraTile.copyTile(*palette_image, i);
  }

  storage.setFilename("vr4/" + mTilesetName);
  string save_png(storage.getFullPath() + "_vr4.png");
  CheckPath(save_png);
  PngExporter::save(save_png, ultraTile, palette, false, false);
}

void TilesetHub::generateCV5Json(Storage storage)
{
  json j_cv5;

  for(auto element : *cv5->array())
  {
    json j_cv5_element;

    json j_megatiles;
    for(auto megatile_ref : *element->vx4_vf4_ref())
    {
      j_megatiles.push_back(megatile_ref);
    }
    j_cv5_element["vx4_vf4_ref"] = j_megatiles;

    j_cv5.push_back(j_cv5_element);
  }

  storage.setFilename("cv5/" + mTilesetName + "_cv5.json");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_cv5, full_path, true);
}

void TilesetHub::generateVF4Json(Storage storage)
{
  json j_vf4;

  for(auto element : *vf4->array())
  {
    json j_vf4_element;

    for(auto flags : *element->flags())
    {
      json j_minitile_flags;
      j_minitile_flags["walkable"] = static_cast<bool>(flags->walkable());
      // TODO: add all other flags
      j_vf4_element.push_back(j_minitile_flags);
    }

    j_vf4.push_back(j_vf4_element);
  }

  storage.setFilename("vf4/" + mTilesetName + "_vf4.json");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_vf4, full_path, true);
}

void TilesetHub::generateVX4Json(Storage storage)
{
  json j_vx4;

  for(auto element : *vx4->array())
  {
    json j_vx4_element;

    for(auto graphic_ref : *element->graphic_ref())
    {
      json j_graphic_ref;
      j_graphic_ref["vr4_ref"] = graphic_ref->vr4_ref();
      j_graphic_ref["horizontal_flip"] = static_cast<bool>(graphic_ref->horizontal_flip());
      j_vx4_element.push_back(j_graphic_ref);
    }

    j_vx4.push_back(j_vx4_element);
  }

  storage.setFilename("vx4/" + mTilesetName + "_vx4.json");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_vx4, full_path, false);
}

void TilesetHub::generateTilesetJson(Storage storage)
{
  if(!cv5) // if it isn't available just return with no action
  {
    return;
  }

  unsigned int num_tiles = vx4->array()->size();
  int tiles_width = MEGATILE_COLUMNS;
  int tiles_height = static_cast<int>(ceil(static_cast<float>(num_tiles) / static_cast<float>(tiles_width)));
  const Size ultra_tile_size = Size(tiles_width, tiles_height);
  const Size image_size = ultra_tile_size * MEGATILE_SIZE;

  json j_tileset;

  j_tileset["columns"] = MEGATILE_COLUMNS;
  j_tileset["image"] = mTilesetName + ".png";
  j_tileset["imageheight"] = image_size.getHeight();
  j_tileset["imagewidth"] = image_size.getWidth();
  j_tileset["margin"] = 0;
  j_tileset["name"] = mTilesetName;
  j_tileset["spacing"] = 0;
  j_tileset["tilecount"] = ultra_tile_size.getHeight() * ultra_tile_size.getWidth();
  j_tileset["tileheight"] = MEGATILE_SIZE.getHeight();
  j_tileset["tilewidth"] = MEGATILE_SIZE.getWidth();
  j_tileset["type"] = "tileset";
  j_tileset["version"] = "1.8";

  storage.setFilename("tiled/" + mTilesetName + ".tsj");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_tileset, full_path, true);

}

void TilesetHub::generateAnimationTilesetJson(unsigned int animation_count, Storage storage)
{
  if(!cv5) // if it isn't available just return with no action
  {
    return;
  }

  int tiles_width = TILE_ANIMATION_FRAMES;
  int tiles_height = animation_count;
  const Size ultra_tile_size = Size(tiles_width, tiles_height);
  const Size image_size = ultra_tile_size * MEGATILE_SIZE;
  unsigned int duration = 200;

  json j_tileset;

  j_tileset["columns"] = TILE_ANIMATION_FRAMES;
  j_tileset["image"] = mTilesetName + "_animation.png";
  j_tileset["imageheight"] = image_size.getHeight();
  j_tileset["imagewidth"] = image_size.getWidth();
  j_tileset["margin"] = 0;
  j_tileset["name"] = mTilesetName + " Animations";
  j_tileset["spacing"] = 0;
  j_tileset["tilecount"] = ultra_tile_size.getHeight() * ultra_tile_size.getWidth();
  j_tileset["tileheight"] = MEGATILE_SIZE.getHeight();
  j_tileset["tilewidth"] = MEGATILE_SIZE.getWidth();
  j_tileset["type"] = "tileset";
  j_tileset["version"] = "1.8";

  unsigned int tileid = 0;
  for(unsigned int ac = 0; ac < animation_count; ac++)
  {
    json j_animation;
    json j_tile;
    j_tile["id"] = tileid;
    for(unsigned int af = 0; af < TILE_ANIMATION_FRAMES; af++)
    {
      json j_animation_frame;
      j_animation_frame["duration"] = duration;
      j_animation_frame["tileid"] = tileid;
      j_animation.push_back(j_animation_frame);
      tileid++;
    }
    j_tile["animation"] = j_animation;


    j_tileset["tiles"].push_back(j_tile);
  }


  storage.setFilename("tiled/" + mTilesetName + "_animation.tsj");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_tileset, full_path, true);

}

void TilesetHub::saveJson(json &j, const std::string &file, bool pretty)
{
  std::ofstream filestream(file);

  if(pretty)
  {
    filestream << std::setw(4) << j;
  }
  else
  {
    filestream << j;
  }
}

const std::string TilesetHub::getTilesetName()
{
  return mTilesetName;
}

std::vector<unsigned int> TilesetHub::getAnimationTiles()
{
  return animation_tiles;
}

unsigned int TilesetHub::getMaxStaticTiles()
{
  /* calculate all tiles of complete image. this adds also empty tiles in the last line */
  float max_tiles_f = static_cast<float>(max_static_tiles) / MEGATILE_COLUMNS + 1;
  int max_tiles_i = static_cast<int>(max_tiles_f) * MEGATILE_COLUMNS;

  return max_tiles_i;
}

} /* namespace tileset */
