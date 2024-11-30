/*
 * Chk.cpp
 *
 *      Author: Andreas Volz
 */

// Local
#include "Chk.h"
#include "Hurricane.h"
#include "FileUtil.h"
#include "Logger.h"
#include "StringUtil.h"

// system
#include <cstring>
#include <cstdint>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace std;
/*

*/
const std::vector<std::string> Chk::TILESET_MAPPING = {
  "badlands",
  "platform",
  "install",
  "ashworld",
  "jungle",
  "dessert",
  "artic",
  "twilight",
};

Chk::Chk(std::shared_ptr<Hurricane> hurricane, const std::string &map_name) :
  Converter(hurricane),
  m_map_name(map_name)
{
  const std::string arcfile = map_name + "\\staredit\\scenario.chk";
  m_chk_parser_stream = mHurricane->extractStream(arcfile);
  assert(m_chk_parser_stream);
  m_chk_parser_ks = make_shared<kaitai::kstream>(&*m_chk_parser_stream);
  assert(m_chk_parser_ks);
  chk_parser = make_shared<chk_parser_t>(m_chk_parser_ks.get());
  assert(chk_parser);
}

Chk::~Chk()
{
}

const std::string Chk::getTileSet()
{
  string tileset_str;

  for(const chk_parser_t::chunk_type_t* chunk : *chk_parser->chunk())
  {
    if(chunk->tag() == "ERA ")
    {
      chk_parser_t::tileset_t *tileset = static_cast<chk_parser_t::tileset_t*>(chunk->data()->content());
      tileset_str = TILESET_MAPPING[tileset->value()];
    }
  }

  return tileset_str;
}

bool Chk::convertTiled(tileset::TilesetHub &tilesethub, Storage storage)
{
  bool result = false;
  string map_name_flat(m_map_name);
  replaceString("\\", "_", map_name_flat);

  auto animation_tiles = tilesethub.getAnimationTiles();

  json j_tilemap;
  j_tilemap["infinite"] = false;
  j_tilemap["compressionlevel"] = -1;
  j_tilemap["tileheight"] = tileset::TilesetHub::MEGATILE_SIZE.getHeight();
  j_tilemap["tilewidth"] = tileset::TilesetHub::MEGATILE_SIZE.getWidth();
  j_tilemap["orientation"] = "orthogonal";
  j_tilemap["type"] = "map";
  j_tilemap["renderorder"] = "right-down";
  j_tilemap["version"] = "1.8";
  j_tilemap["tiledversion"] = "1.8.0";

  json j_tilesets_ref;
  j_tilesets_ref["firstgid"] = 1;
  j_tilesets_ref["source"] = "../tileset/tiled/" + getTileSet() + ".tsj";
  j_tilemap["tilesets"].push_back(j_tilesets_ref);

  // add animation reference only if animation tiles in the map
  if(animation_tiles.size() > 0)
  {
    json j_tilesets_anim_ref;
    j_tilesets_anim_ref["firstgid"] = tilesethub.getMaxStaticTiles() + 1;
    j_tilesets_anim_ref["source"] = "../tileset/tiled/" + getTileSet() + "_animation.tsj";
    j_tilemap["tilesets"].push_back(j_tilesets_anim_ref);
  }

  json j_layer_0;
  j_layer_0["id"] = 1;
  j_layer_0["name"] = map_name_flat + " Layer";
  j_layer_0["type"] = "tilelayer";
  j_layer_0["visible"] = true;
  j_layer_0["x"] = 0;
  j_layer_0["y"] = 0;
  j_layer_0["opacity"] = 1;

  json j_layer_data;
  for(const chk_parser_t::chunk_type_t* chunk : *chk_parser->chunk())
  {
   if(chunk->tag() == "DIM ")
    {
      chk_parser_t::dimension_t *dimension = static_cast<chk_parser_t::dimension_t*>(chunk->data()->content());
      j_tilemap["height"] = dimension->height();
      j_tilemap["width"] = dimension->width();
      j_layer_0["height"] = dimension->height();
      j_layer_0["width"] = dimension->width();
    }
    else if(chunk->tag() == "MTXM")
    {
      chk_parser_t::u2_array_t *terrain_array = static_cast<chk_parser_t::u2_array_t*>(chunk->data()->content());

      for(uint16_t terrain : *terrain_array->values())
      {
        uint16_t groupIndex = (terrain & 0x7FF0) >> 4;
        uint16_t tileIndex = terrain & 0x000F;

        tileset_cv5_t::group_t* group = tilesethub.cv5->array()->at(groupIndex);
        unsigned int megatile_ref = group->vx4_vf4_ref()->at(tileIndex);

        auto found_it = std::find(animation_tiles.begin(), animation_tiles.end(), megatile_ref);

        if(found_it == animation_tiles.end())
        {
          // case for non-animation tile
          j_layer_data.push_back(megatile_ref + 1); // +1 needed as Tiled always resets firstgid to 1...
        }
        else
        {
          // add animation reference only if animation tiles in the map
          if(animation_tiles.size() > 0)
          {
            // case for animation tile
            unsigned int tiles_count = tilesethub.getMaxStaticTiles();
            auto index = std::distance(animation_tiles.begin(), found_it);
            unsigned int absolute_index = tiles_count + index * tileset::TilesetHub::TILE_ANIMATION_FRAMES + 1; // +1 needed as Tiled always resets firstgid to 1...
            j_layer_data.push_back(absolute_index);
          }
        }
      }
    }
  }

  j_layer_0["data"] = j_layer_data;

  j_tilemap["layers"].push_back(j_layer_0);

  storage.setFilename(map_name_flat + ".tmj");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_tilemap, full_path, true);

  result = true;
  return result;
}

void Chk::generateMapJson(tileset::TilesetHub &tilesethub, Storage storage)
{
  string map_name_flat(m_map_name);
  replaceString("\\", "_", map_name_flat);

  json j_tilemap;
  json j_mtxm_data;
  json j_dim_data;

  for(const chk_parser_t::chunk_type_t* chunk : *chk_parser->chunk())
  {
   if(chunk->tag() == "DIM ")
    {
      chk_parser_t::dimension_t *dimension = static_cast<chk_parser_t::dimension_t*>(chunk->data()->content());
      j_dim_data["height"] = dimension->height();
      j_dim_data["width"] = dimension->width();
    }
    else if(chunk->tag() == "MTXM")
    {
      chk_parser_t::u2_array_t *terrain_array = static_cast<chk_parser_t::u2_array_t*>(chunk->data()->content());

      for(uint16_t terrain : *terrain_array->values())
      {
        // store megatile link direct
        j_mtxm_data.push_back(terrain);
      }
    }
  }

  j_tilemap["MTXM"] = j_mtxm_data;
  j_tilemap["DIM"] = j_dim_data;

  storage.setFilename(map_name_flat + "_chk.json");
  string full_path = storage.getFullPath();
  CheckPath(full_path);
  saveJson(j_tilemap, full_path, true);
}

void Chk::saveJson(json &j, const std::string &file, bool pretty)
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
