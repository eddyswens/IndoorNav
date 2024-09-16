#ifndef __MEASURE_TYPES_H__
#define __MEASURE_TYPES_H__

#include <stdint.h>

/* x,y,z vector */
struct vec3_s {
  uint32_t timestamp; // Timestamp when the data was computed

  float x;
  float y;
  float z;
};

typedef struct vec3_s point_t;


typedef struct tdoaMeasurement_s {
  union {
    point_t anchorPositions[2];
    struct {
      point_t anchorPositionA;
      point_t anchorPositionB;
    };
  };
  union {
    uint8_t anchorIds[2];
    struct {
      uint8_t anchorIdA;
      uint8_t anchorIdB;
    };
  };

  float distanceDiff;
  float stdDev;
} tdoaMeasurement_t;


#endif
