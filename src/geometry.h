#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

bool CheckCollisionLineRect(Vector2 A, Vector2 B, Rectangle rect, Vector2* intersection);

#endif
