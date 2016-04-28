#ifndef VALUENOISE
#define VALUENOISE

#include "util/CommonIncludes.h"

class ValueNoise
{
public:
    ValueNoise(int octaves, double persistence, int seed);
    ~ValueNoise();

    double perlin(double x, double y);

private:
    double m_octaves;
    double m_persistence;
    int m_seed;

    double interpolate(double a, double b, double w);

    double noise(int x, int y);

    double smoothNoise(double x, double y);

    double interpolatedNoise(double x, double y);
};

#endif // VALUENOISE

