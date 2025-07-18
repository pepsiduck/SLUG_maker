#include "geometry.h"

bool CheckCollisionLineRect(Vector2 A, Vector2 B, Rectangle rect, Vector2* intersection)
{
    if(CheckCollisionPointRec(A,rect))
        return true;
    if(CheckCollisionPointRec(B,rect))
        return true;
    Vector2 direction = Vector2Subtract(B,A);
    if(direction.x == 0.0 && direction.y == 0.0)
        return false;

    float minParam = -INFINITY, maxParam = INFINITY;
    if(direction.x != 0.0)
	{
		float txMin = (rect.x - A.x) / direction.x;
		float txMax = ((rect.x + rect.width) - A.x) / direction.x;

		minParam = fmax(minParam, fmin(txMin, txMax));
		maxParam = fmin(maxParam, fmax(txMin, txMax));
	}
    else if((A.x >= rect.x) && (A.x <= rect.x + rect.width) && ((A.y >= rect.y) ^ (B.y >= rect.y)))
    {
        return true; 
    }  
    else
    {
        return false;
    }

	if(direction.y != 0.0)
	{
		float tyMin = (rect.y - A.y) / direction.y;
		float tyMax = ((rect.y + rect.height) - A.y) / direction.y;

		minParam = fmax(minParam, fmin(tyMin, tyMax));
		maxParam = fmin(maxParam, fmax(tyMin, tyMax));
	}
    else if((A.y >= rect.y) && (A.y <= rect.y + rect.height) && ((A.x >= rect.x) ^ (B.x >= rect.x)))
    {
        return true;    
    }    
    else
    {
        return false;
    }
	// if maxParam < 0, ray is intersecting AABB, but the whole AABB is behind us
	if(maxParam < 0)
    {
		return false;
    }


	// if minParam > maxParam, ray doesn't intersect AABB
	if(minParam > maxParam)
		return false;
    
    if((A.y >= rect.y) == (B.y >= rect.y))
    {
        if((A.x >= rect.x) == (B.x >= rect.x))
        {
            if((A.y >= rect.y + rect.height) == (B.y >= rect.y + rect.height))
            {
                if((A.x >= rect.x + rect.width) == (B.x >= rect.x + rect.width))
                    return false;
            }
        }
    }
    
	if(intersection != NULL)
		*intersection = Vector2Add(A, Vector2Scale(direction, minParam));

	return true;
}

float Vector2CrossProduct(Vector2 A, Vector2 B)
{
    return (A.x * B.y) - (A.y * B.x);
}

int32_t Vector2CrossProduct(int32_t ax, int32_t ay, int32_t bx, int32_t by)
{
    return ax*by - ay*bx;
}
