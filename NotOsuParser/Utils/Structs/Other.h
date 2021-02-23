#pragma once

struct BeatmapPoint
{
    BeatmapPoint()
    {
        x = 0;
        y = 0;
    }
    
    BeatmapPoint(const int x, const int y)
    {
        this->x = x;
        this->y = y;
    }
    
    int x;
    int y;
};