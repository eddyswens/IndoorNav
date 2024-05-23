#pragma once
#include "Anchor.h"

class PosEstimator {
    Anchor *anchors = new Anchor[ancQuantt];
    double mobPos[3];

public:
    inline PosEstimator () {
        /* здесь задаются координаты первого анкера, остальные
        автоматически задаются по квадрату */
        mobPos[0] = 1.0;
        mobPos[1] = 1.0;
        mobPos[2] = 2;
        int rotate = 1;
        for (int i = 0; i < ancQuantt; i ++) {
            *(anchors + i) = Anchor(mobPos);
            mobPos[0] *= rotate;
            mobPos[1] *= -rotate;
            rotate *= -1;
        }
    }

    inline void posCalc () { 
        double x[4];
        for (int i = 0; i < ancQuantt; i++) { x[i] = *(anchors[i].getPos()); }

        double y[4];
        for (int i = 0; i < ancQuantt; i++) { y[i] = *(anchors[i].getPos() + 1); }

        double HtH[3]; // {{0, 1},{1, 2}}
        HtH[0] = 3 * x[0] * x[0] - 2 * x[0] * (x[1] + x[2] + x[3]) + 
                 x[1] * x[1] + x[2] * x[2] + x[3] * x[3];

        HtH[1] = 3 * x[0] * y[0] - x[0] * (y[1] + y[2] + y[3]) +
                 x[1] * (y[1] - y[0]) + x[2] * (y[2] - y[0]) + x[3] * (y[3] - y[0]);

        HtH[2] = 3 * y[0] * y[0] - 2 * y[0] * (y[1] + y[2] + y[3]) + 
                 y[1] * y[1] + y[2] * y[2] + y[3] * y[3];

        double det = HtH[0] * HtH[2] - HtH[1] * HtH[1];

        if (det == 0) { return; }

        double invHtH[3] = {HtH[2] / det, -HtH[1] / det, HtH[0] / det};
        double invHtH_Ht[6];

        for (int i = 1; i < 4; i++) {
            invHtH_Ht[i - 1] = invHtH[0] * (x[0] - x[i]) + invHtH[1] * (y[0] - y[i]);
            invHtH_Ht[i + 2] = invHtH[1] * (x[0] - x[i]) + invHtH[2] * (y[0] - y[i]);
        }
        
        double R[3];
        for(int i = 1; i < 4; i++) {
            R[i - 1] = *(anchors[i].getDistance()) * *(anchors[i].getDistance()) - 
                       *(anchors[0].getDistance()) * *(anchors[0].getDistance()) +
                       x[0] * x[0] - x[i] * x[i] + y[0] * y[0] - y[i] * y[i];
            R[i - 1] /= 2;
        }

        mobPos[0] = mobPos[1] = 0;
        for(int i = 0; i < 3; i++) {
            mobPos[0] += invHtH_Ht[i] * R[i];
            mobPos[1] += invHtH_Ht[i + 3] * R[i];
        }  
        
        double leg[2]; 
        double b0x = *(anchors[0].getPos()); 
        double b0y = *(anchors[0].getPos() + 1); 
        int reflect[8] = {1, 1, 1, -1, -1, -1, -1, 1}; 
        mobPos[2] = 0; 

        for (int i = 0; i < ancQuantt; i++) {
            leg[0] = b0x - reflect[2 * i] * mobPos[0]; 
            leg[1] = b0y - reflect[2 * i + 1] * mobPos[1]; 
            double nextLeg = *(anchors[i].getDistance()) * *(anchors[i].getDistance()) - 
                             (leg[0] * leg[0] + leg[1] * leg[1]);
            if (nextLeg < 0) { return; }
            mobPos[2] += *(anchors[0].getPos() + 2) - sqrt(nextLeg);
        }
        mobPos[2] /= 4;
    }

    inline void setAnchorsDist (double dist[4]) {
        for (int i = 0; i < ancQuantt; i++) { anchors[i].setDistance(dist[i]); }
    }

    inline double* getMobPos () { return mobPos; } //success
};