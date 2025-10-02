//
// Created by souvik on 10/2/25.
//

#include "tile_manager.h"
#include <cstring>


std::vector<Tile> TileManager::split(unsigned char* shm_ptr,
                                     int width, int height, int channels,
                                     int num_tiles)
{
    std::vector<Tile> tiles;
    if (num_tiles <= 0) num_tiles = 1;
    int rows_per_tile = height / num_tiles;

    for (int i = 0; i < num_tiles; ++i) {
        int start = i * rows_per_tile;
        int end = (i == num_tiles - 1) ? height : start + rows_per_tile;
        unsigned char* ptr = shm_ptr + static_cast<size_t>(start) * width * channels;
        tiles.push_back({start, end, ptr});
    }

    return tiles;
}

void TileManager::join(const std::vector<Tile>& tiles,
                       unsigned char* output,
                       int width, int height, int channels)
{
    for (const auto& tile : tiles) {
        int tile_height = tile.end_row - tile.start_row;
        unsigned char* dst = output + static_cast<size_t>(tile.start_row) * width * channels;
        std::memcpy(dst, tile.data, static_cast<size_t>(tile_height) * width * channels);
    }
}