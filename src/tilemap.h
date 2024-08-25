#pragma once

#include <optional>
#include "animation.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "assetManager.h"
#include "tileset.h"
#include "math.h"
#include "pugixml.hpp"
#include <cassert>
#include "LDtkParser.h"

typedef std::map<std::string, std::string> TiledLayer;

struct TilesetTile {
  Rect textureRect;
  // @TODO: put custom properties here
};

/*
class Tileset {
  std::vector<TilesetTile> _tiles;
  SDL_Texture* _texture;
public:
  int getTileWidth();
  int getTileHeight();
};
*/

// Rename this to TileObject. Not Tiled specific
struct TiledObject {
  std::string name;
  v2i position;

  std::optional<float> width = NULL;
  std::optional<float> height = NULL;

  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, std::string> propertiesString;
};

/*
class Tile {
  v2i position;
};

// @TODO: create these in z-order
class TileLayer {
  public:
    /// nullptr means no tile
    std::vector<Tile*> tiles;
}r
*/

// @TODO: create these in z-order
class TileLayer {
  public:
    /// ID 0 means no tile
    std::vector<int> tiles;
};

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";

  public:
    Tilemap(int firstGID, int tilesWide, int tilesTall, Tileset tileset, std::vector<TileLayer> layers, std::vector<TiledObject> objects);

    //Tilemap(LDTK_Level ldtkLevel);

    ~Tilemap() {
    }

    int getTileWidth() {
      return _tileset.getTileWidth();
    }

    int getTileHeight() {
      return _tileset.getTileHeight();
    }

    // @TODO: is this needed?
    int getWidthInPixels() {
      return _tilesWide * getTileWidth();
    }

    // @TODO: is this needed?
    int getHeightInPixels() {
      return _tilesTall * getTileHeight();
    }

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

    bool isOutsideMap(int x, int y) {
      return (
        x < 0 || y < 0 ||
        x > _tilesWide * getTileWidth() || y > _tilesTall * getTileHeight()
      );
    }

    std::vector<int> getIndicesWithinRect(RectF r, int layer);

    /// Returns -1 if empty tile
    int getIdxFromPoint(int x, int y, int layer);

    Rect getTileRect(int layerIdx, int idx);

    std::vector<TileLayer>* getLayers() {
      return &_layers;
    }

    TileData* getTileDataByIdx(int idx, int layer) {
      assert(layer <= _layers.size());
      assert(idx <= _layers.at(layer).tiles.size());

      int id = _layers.at(layer).tiles.at(idx);
      return _tileset.getTileData(id - _firstGID);
    }

    TileData* getTileData(int id) {
      return _tileset.getTileData(id - _firstGID);
    }

    Tileset* getTileset() {
      return &_tileset;
    }
  private:
    int _firstGID;
    int _tilesWide;
    int _tilesTall;

    std::vector<TiledObject> _objects;
    Tileset _tileset;
    std::vector<TileLayer> _layers;
};
