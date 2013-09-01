#ifndef __BLOCK_H__
#define __BLOCK_H__

enum BlockType
{
    BT_BLANK,

    // water
    BT_WATER_SHALLOW,
    BT_WATER_DEEP,

    // grassland
//    BT_GRASS,
//    BT_BARREN,

    // roads
    BT_ROAD_VERTICAL,
    BT_ROAD_HORIZONTAL,
    BT_ROAD_UPLEFT,
    BT_ROAD_DOWNRIGHT,
    BT_ROAD_DOWNLEFT,
    BT_ROAD_UPLEFT,

    // forests
    BT_FOREST_LIGHT,
    BT_FOREST_NORMAL,
    BT_FOREST_HEAVY,

    // ladders 
    BT_LADDER_UP,
    BT_LADDER_DOWN,

    // mountains
    BT_MOUNTAIN_LOW,
    BT_MOUNTAIN_NORMAL,
    BT_MOUNTAIN_HIGH,

    // coastline
    BT_COASTLINE_LEFT,
    BT_COASTLINE_RIGHT,
    BT_COASTLINE_TOP,
    BT_COASTLINE_BOTTOM,

    BT_LASTBLOCK
};


#endif
