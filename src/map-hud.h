#pragma once
#include "renderer.h"
#include "map.h"

#define MAP_HUD_CELL_WIDTH 10
#define MAP_HUD_CELL_HEIGHT 8

enum class Direction {
  North,
  West,
  South,
  East,
};

enum class BorderValue {
  Wall,
  Open,
  Passage,
};

enum class CellType {
  Normal,
  Save,
  Teleport,
};

enum class Marker {
  None,
  Player,
};

struct MapCell {
  v2i position;
  v2i size;
  CellType type;

  std::string iid;

  BorderValue north = BorderValue::Open;
  BorderValue east = BorderValue::Open;
  BorderValue south = BorderValue::Open;
  BorderValue west = BorderValue::Open;
};

class MapHud {
private:
  Renderer* _renderer;
  v2i _position;
  std::vector<MapCell> _cells;
  void drawCell(MapCell cell, Marker marker, v2i playerTilePosition);

  int _worldCellWidth;
  int _worldCellHeight;
public:
  MapHud(Renderer* renderer, World* world, v2i position);
  void draw(std::string currentLevel, v2i playerTilePosition);
};
