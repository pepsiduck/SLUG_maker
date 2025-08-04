#include "geometry.h"

float GetSegmentExtendedSide(SLUG_SegmentExtended *seg, Vector2 *p)
{
    return Vector2DotProduct(seg->normal, *p) - seg->dist;
}

int8_t SegmentExtendedIntersect(SLUG_SegmentExtended *seg1, SLUG_SegmentExtended *seg2)
{
    if(seg1 == NULL || seg2 == NULL)
        return -1;

    float a = GetSegmentExtendedSide(seg1, &(seg2->A));
    float b = GetSegmentExtendedSide(seg1, &(seg2->B));

    if(a == 0.0 && b == 0.0)
    {
        if(Vector2DotProduct(Vector2Subtract(seg2->A, seg1->A), Vector2Subtract(seg2->A, seg1->B)) <= 0.0)
        {
            if(Vector2DotProduct(Vector2Subtract(seg2->B, seg1->A), Vector2Subtract(seg2->
B, seg1->B)) <= 0.0)
                return 1;
            else
                return 0;
        }
        else 
            return 0;
    }

    if(a * b >= 0.0)
        return 0;

    a = GetSegmentExtendedSide(seg2, &(seg1->A));
    b = GetSegmentExtendedSide(seg2, &(seg1->B));

    if(a * b >= 0.0)
        return 0;
        
    return 1;
}

int8_t SegmentExtendedIntersectArray(SLUG_SegmentExtended *segs, int32_t seg_size)
{
    if(segs == NULL || seg_size <= 0)
        return -1;
    for(int32_t i = 0; i < seg_size - 1; ++i)
    {
        for(int32_t j = i + 1; j < seg_size; ++j)
        {
            if(SegmentExtendedIntersect(&(segs[i]), &(segs[j])))
                return 1;
        }
    }
    return 0;
}

int8_t NoSegmentsPoints(SLUG_SegmentExtended *segs, int32_t seg_size)
{
    if(segs == NULL || seg_size <= 0)
        return -1;
    for(int32_t i = 0; i < seg_size; ++i)
    {
        if(segs[i].A.x == segs[i].B.x && segs[i].A.y == segs[i].B.y)
            return 0;
    }
    return 1;
}

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

float Vector2CrossProductV(Vector2 A, Vector2 B)
{
    return (A.x * B.y) - (A.y * B.x);
}

int32_t Vector2CrossProductInt(int32_t ax, int32_t ay, int32_t bx, int32_t by)
{
    return ax*by - ay*bx;
}
