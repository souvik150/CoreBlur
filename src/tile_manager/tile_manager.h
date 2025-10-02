//
// Created by souvik on 10/2/25.
//

#ifndef MYPROJECT_TILE_MANAGER_H
#define MYPROJECT_TILE_MANAGER_H
#include <vector>

struct Tile {
    int start_row;
    int end_row;
    unsigned char* data;
};

class TileManager {
public:
    static std::vector<Tile> split(unsigned char* shm_ptr,
                                   int width, int height, int channels,
                                   int num_tiles);

    static void join(const std::vector<Tile>& tiles,
                     unsigned char* output,
                     int width, int height, int channels);
};


#endif //MYPROJECT_TILE_MANAGER_H