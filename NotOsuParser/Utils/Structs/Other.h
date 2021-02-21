#pragma once

struct BeatmapPoint
{
    BeatmapPoint(const int x, const int y)
    {
        this->x = x;
        this->y = y;
    }
    
    int x;
    int y;
};