#pragma once
#include <cmath>

enum { 
    ancQuantt = 4,
};

class Anchor { 
    double pos[3];
    double distance;

public:
    inline Anchor () {} 

    inline Anchor (double pos[3]) { 
        for (int i = 0; i < 3; i++){ this->pos[i] = pos[i]; }
    }

    inline void setDistance (double distance) {
        this->distance = distance;
    }

    inline double* getDistance () { return &distance; }

    inline double* getPos () { return pos; }
};