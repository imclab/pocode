/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include <cmath>
#include <ctime>
#include <algorithm>

#include "poPoint.h"
#include "poRect.h"
#include "poColor.h"

#define M_2PI (M_PI*2.0)
#define M_HALF_PI (M_PI/2.0)
#define M_QUARTER_PI (M_HALF_PI/2.0)

inline bool compare(float f1, float f2, float range=1.0e-6f) {
	return ::fabs(f1-f2) < range;
}

inline float deg2rad(float deg) {
	return deg / 180.0 * M_PI;
}

inline float rad2deg(float rad) {
	return rad / M_PI * 180.0;
}

inline float sin_deg(float deg) {
	return ::sinf(deg2rad(deg));
}

inline float cos_deg(float deg) {
	return ::cosf(deg2rad(deg));
}

inline void poRandSeed() {
	::srand((unsigned int)::time(NULL));
}

inline void poRandSeed(int seed) {
	::srand(seed);
}

inline float poRand() {
	return rand() / (float)INT_MAX;
}

inline float poRand(float low, float high) {
	return poRand() * (high-low) + low;
}

inline poPoint randomPointInRect(poRect r) {
	return poPoint(poRand(r.origin.x,r.origin.x+r.size.x),
				   poRand(r.origin.y,r.origin.y+r.size.y));
}

inline poColor randomColor(float alpha=1.f) {
	return poColor(poRand(), poRand(), poRand(), alpha);
}

inline void capToRange(float *f, float low, float high) {
	*f = std::max(*f, low);
	*f = std::min(*f, high);
}

inline void capToRange(poPoint *pt, float low, float high) {
	capToRange(&pt->x, low, high);
	capToRange(&pt->y, low, high);
	capToRange(&pt->z, low, high);
}

// will round -.5 to -1
inline float roundSym(float f) {
	return (f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5);
}

inline poPoint roundSym(poPoint p) {
	return poPoint(roundSym(p.x), roundSym(p.y), roundSym(p.z));
}

inline float round(float f) {
	return floor(f + 0.5);
}

inline poPoint round(poPoint p) {
	return poPoint(round(p.x), round(p.y), round(p.z));
}

inline poPoint floor(poPoint p) {
	return poPoint(floor(p.x), floor(p.y), floor(p.z));
}

template <typename T>
inline T clamp(T min, T max, T val) {
	return std::max(std::min(val, max), min);
}

inline float poMapf(float min1, float max1, float v, float min2, float max2) {
	float tmp = (v-min1) / (max1-min1);
	return tmp * (max2-min2) + min2;
}

struct poRay {
	poPoint origin, dir;
	poRay(poPoint o, poPoint d)
	: origin(o), dir(normalize(d)) {}
};

// angle ABC
float angleBetweenPoints(poPoint a, poPoint b, poPoint c);
// returns false if parallel, p1 = intersection or p1 and p2 = closest point on each
bool rayIntersection(poRay r1, poRay r2, poPoint *p1, poPoint *p2);

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C );

