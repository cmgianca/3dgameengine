#include "engine/common/noise/valuenoise.h"
#include "glm/gtc/random.hpp"

ValueNoise::ValueNoise(int octaves, double persistence, int seed) :
    m_octaves(octaves),
    m_persistence(persistence),
    m_seed(seed)
{
}

ValueNoise::~ValueNoise() {

}

double ValueNoise::interpolate(double a, double b, double w) {
    double ft = w*3.1415927;
    double f = (1-cos(ft))*.5;
    return a*(1-f) + b*f;
}

double ValueNoise::noise(int x, int y) {
    unsigned n = x + y * 57 + 13*m_seed;
    n = (n<<13) ^ n;
    return (1.-((n*(n*n*15731+789221)+1376312589) & 2147483647)/ 1073741824.0);
}

double ValueNoise::smoothNoise(double x, double y){
    double corners = (noise(x-1,y-1)+noise(x+1,y-1)+noise(x-1,y+1)+noise(x+1,y+1))/16.f;
    double sides = (noise(x-1,y)+noise(x+1,y)+noise(x,y-1)+noise(x,y+1))/8.f;
    double center = noise(x,y)/4.f;
    return corners + sides + center;
}

double ValueNoise::interpolatedNoise(double x, double y) {
    int ix = floor(x);
    int iy = floor(y);

    double fx = x-ix;
    double fy = y-iy;

    double v1 = smoothNoise(ix, iy);
    double v2 = smoothNoise(ix+1, iy);
    double v3 = smoothNoise(ix, iy+1);
    double v4 = smoothNoise(ix+1, iy+1);

    double i1 = interpolate(v1, v2, fx);
    double i2 = interpolate(v3, v4, fx);

    return interpolate(i1, i2, fy);
}

double ValueNoise::perlin(double x, double y) {
    double total = 0;

    for (int i = 0; i < m_octaves; i++) {
        double frequency = pow(2,i);
        double amplitude = pow(m_persistence, i);

        total = total + interpolatedNoise(x*frequency, y*frequency) * amplitude;
    }

    return total;
}





//    unsigned x1 = x*chunkSize;
//    unsigned y1 = z*chunkSize;
//    unsigned mid = chunkSize/2;

//    float range = 4;
//    float roughness = 2;
//    GLfloat toReturn[(chunkSize+1)*(chunkSize+1)] = {};

//    float a = toReturn[0] = 16+random(x1,y1)*4;
//    float b = toReturn[chunkSize*(chunkSize+1)] =  16+random(x1+chunkSize,y1)*4;
//    float c = toReturn[chunkSize] = 16+random(x1,y1+chunkSize)*4;
//    float d = toReturn[chunkSize*(chunkSize+1) + chunkSize] = 16+random(x1+chunkSize,y1+chunkSize)*4;
//    float e = toReturn[mid*(chunkSize+1)+mid] = (a+b+c+d)/4 + random(x1+mid,y1+mid)*range*2;
//    toReturn[mid*(chunkSize+1)] = (a+c+e)/3 + random(x1,y1+mid)*range*2;
//    toReturn[mid*(chunkSize+1)+chunkSize] = (a+b+e)/3 + random(x1+mid,y1)*range*2;
//    toReturn[mid] = (b+d+e)/3 + random(x1+chunkSize,y1+mid)*range*2;
//    toReturn[chunkSize*(chunkSize+1)+mid] = (c+d+e)/3 + random(x1+mid,y1+chunkSize)*range*2;

//void WorldChunk::DiamondSquare(unsigned x1, unsigned y1, unsigned x2, unsigned y2, float range, float roughness, int dimensions, GLfloat* map){
//    int level = (x2-x1)/4;
//    int loops = 0;

//    while (level >= 1) {
//        //Diamonds
//        for (unsigned x = level; x+level <= x2-x1; x += 2*level) {
//            for (unsigned y = level; y+level <= y2-y1; y += 2*level) {
//                map[x*(dimensions+1)+y] = getDiamondAverage(x,y,level,dimensions, map) + random(x1+x,y1+y)*range;
//            }
//        }
//        //Squares
//        for (unsigned x = level; x+level <= x2-x1; x += 2*level) {
//            for (unsigned y = level; y+level <= y2-y1; y += 2*level) {
//                map[(x-level)*(dimensions+1)+y] = getSquareAverage(x-level,y,level,dimensions, map) + random(x1+x-level,y2+y)*range;
//                map[x*(dimensions+1)+(y-level)] = getSquareAverage(x,y-level,level,dimensions, map) + random(x1+x,y2+y-level)*range;
//                map[(x+level)*(dimensions+1)+y] = getSquareAverage(x+level,y,level,dimensions, map) + random(x1+x+level,y2+y)*range;
//                map[x*(dimensions+1)+(y+level)] = getSquareAverage(x,y+level,level,dimensions, map) + random(x1+x,y2+y+level)*range;
//            }
//        }
//        loops++;
//        level = level/2;
//        range *= pow(2,-roughness);
//    }
//}

//GLfloat WorldChunk::getDiamondAverage(int x, int y, int distance, int dimensions, GLfloat* map) {
//    int zeroCount = 0;
//    GLfloat p1,p2,p3,p4;
//    if (x-distance < 0 || y-distance < 0) {
//        zeroCount++;
//        p1 = 0;
//    } else {
//        p1 = map[(x-distance)*(dimensions+1)+y-distance];
//    }
//    if (x+distance > dimensions || y-distance < 0) {
//        zeroCount++;
//        p2 = 0;
//    } else {
//        p2 = map[(x+distance)*(dimensions+1)+y-distance];
//    }
//    if (x-distance < 0 || y+distance > dimensions) {
//        zeroCount++;
//        p3 = 0;
//    } else {
//        p3 = map[(x-distance)*(dimensions+1)+y+distance];
//    }
//    if (x+distance > dimensions || y+distance > dimensions) {
//        zeroCount++;
//        p4 = 0;
//    } else {
//        p4 = map[(x+distance)*(dimensions+1)+y+distance];
//    }
//    if (zeroCount >= 1) {
//        std::cout << "ERROR- Diamond Coords: " << x << "," << y << std::endl;
//        std::cout << "Point 1: " << p1 << std::endl;
//        std::cout << "Point 2: " << p2 << std::endl;
//        std::cout << "Point 3: " << p3 << std::endl;
//        std::cout << "Point 4: " << p4 << std::endl;
//        return 0;
//    } else {
//        return (p1+p2+p3+p4)/4;
//    }
//}

//GLfloat WorldChunk::getSquareAverage(int x, int y, int distance, int dimensions, GLfloat* map) {
//    int zeroCount = 0;
//    GLfloat p1,p2,p3,p4;
//    if (x-distance < 0) {
//        zeroCount++;
//        p1 = 0;
//    } else {
//        p1 = map[(x-distance)*(dimensions+1)+y];
//    }
//    if (y-distance < 0) {
//        zeroCount++;
//        p2 = 0;
//    } else {
//        p2 = map[x*(dimensions+1)+y-distance];
//    }
//    if (x+distance > dimensions) {
//        zeroCount++;
//        p3 = 0;
//    } else {
//        p3 = map[(x+distance)*(dimensions+1)+y];
//    }
//    if (y+distance > dimensions) {
//        zeroCount++;
//        p4 = 0;
//    } else {
//        p4 = map[x*(dimensions+1)+y+distance];
//    }
//    if (zeroCount > 1) {
//        std::cout << "ERROR- Square Coords: " << x << "," << y << std::endl;
//        std::cout << "Point 1: " << p1 << std::endl;
//        std::cout << "Point 2: " << p2 << std::endl;
//        std::cout << "Point 3: " << p3 << std::endl;
//        std::cout << "Point 4: " << p4 << std::endl;
//        return 0;
//    } else {
//        return (p1+p2+p3+p4)/(4-zeroCount);
//    }
//}

