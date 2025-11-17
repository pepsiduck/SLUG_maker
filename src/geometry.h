#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct SLUG_SegmentExtended SLUG_SegmentExtended;
struct SLUG_SegmentExtended
{
    Vector2 A; 
    Vector2 B;
    Vector2 normal; //||norm|| = 1
    float dist; //distance de la droite représentée par le segment par rapport à 0, 0
};

typedef struct SLUGmaker_SegmentExtended SLUGmaker_SegmentExtended;
struct SLUGmaker_SegmentExtended
{
    Vector2 A; 
    Vector2 B;
    Vector2 normal; //||norm|| = 1
    Vector2 middlepoint;
    SLUGmaker_SegmentExtended *next;
    bool exists;
};

float GetSegmentExtendedSide(SLUG_SegmentExtended *seg, Vector2 *p);
int8_t SegmentExtendedIntersect(SLUG_SegmentExtended *seg1, SLUG_SegmentExtended *seg2);
int8_t SegmentExtendedIntersectArray(SLUG_SegmentExtended *segs, int32_t seg_size);
int8_t NoSegmentsPoints(SLUG_SegmentExtended *segs, int32_t seg_size);
bool CheckCollisionLineRect(Vector2 A, Vector2 B, Rectangle rect, Vector2* intersection);
float Vector2CrossProductV(Vector2 A, Vector2 B);
int32_t Vector2CrossProductInt(int32_t ax, int32_t ay, int32_t bx, int32_t by);
int8_t RectangleMultiply(Rectangle *rect, float factor_x, float factor_y);

#endif
