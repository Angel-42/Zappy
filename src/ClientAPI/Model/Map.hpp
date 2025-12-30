#pragma once
#include <vector>
#include <memory>
#include "Tile.hpp"

namespace clientapi {

/**
 * @brief Represents the game map as a 2D grid of tiles.
 * 
 * Provides access to individual tiles and allows updating their contents.
 */
class Map {
public:
    /**
     * @brief Construct a new Map object
     * @param width The width of the map
     * @param height The height of the map
     */
    Map(int width, int height);
    /**
     * @brief Get the width of the map
     * @return The width in tiles
     */
    int getWidth() const;
    /**
     * @brief Get the height of the map
     * @return The height in tiles
     */
    int getHeight() const;
    /**
     * @brief Get a pointer to a tile at (x, y)
     * @param x The x coordinate
     * @param y The y coordinate
     * @return Pointer to the Tile, or nullptr if out of bounds
     */
    Tile* getTile(int x, int y);
    /**
     * @brief Get a const pointer to a tile at (x, y)
     * @param x The x coordinate
     * @param y The y coordinate
     * @return Const pointer to the Tile, or nullptr if out of bounds
     */
    const Tile* getTile(int x, int y) const;
    /**
     * @brief Set the contents of a tile at (x, y)
     * @param x The x coordinate
     * @param y The y coordinate
     * @param tile The Tile object to set
     */
    void setTile(int x, int y, const Tile& tile);

private:
    int _width, _height;
    std::vector<std::vector<Tile>> _tiles;
};

} // namespace clientapi 