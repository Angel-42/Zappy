#include "Map.hpp"
#include <stdexcept>

namespace clientapi {

Map::Map(int width, int height) : _width(width), _height(height), _tiles(height, std::vector<Tile>(width, Tile(0, 0))) {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            _tiles[y][x] = Tile(x, y);
}

int Map::getWidth() const { return _width; }
int Map::getHeight() const { return _height; }

Tile* Map::getTile(int x, int y) {
    if (x < 0 || y < 0 || x >= _width || y >= _height) return nullptr;
    return &_tiles[y][x];
}
const Tile* Map::getTile(int x, int y) const {
    if (x < 0 || y < 0 || x >= _width || y >= _height) return nullptr;
    return &_tiles[y][x];
}
void Map::setTile(int x, int y, const Tile& tile) {
    if (x < 0 || y < 0 || x >= _width || y >= _height) throw std::out_of_range("Map::setTile out of range");
    _tiles[y][x] = tile;
}

} // namespace clientapi 