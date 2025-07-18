#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

bool CheckCollisionLineRect(Vector2 A, Vector2 B, Rectangle rect, Vector2* intersection);
float Vector2CrossProduct(Vector2 A, Vector2 B);
int32_t Vector2CrossProduct(int32_t ax, int32_t ay, int32_t bx, int32_t by);

#endif
