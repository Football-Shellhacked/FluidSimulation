#ifndef PHYSICS_H
#define PHYSICS_H

#include "Particle.h"

extern Particle* particles;

class Physics{

    public:
        static float TARGET_DENSITY;
        static float PRESSURE_MULTIPLIER;
        static float* DENSITIES;
        static float* PROPERTIES;
        static float SmoothingKernel(float dst, float radius);
        static float SmoothingKernelDerivative(float dst, float radius);
        static float CalculateDensity(Particle* samplePoint);
        static float CalculateProperty(Particle* samplePoint);
        static Vector2 CalculatePropertyGradient(Particle* samplePoint);
        static float ConvertDensityToPressure(float density);
        static Vector2 CalculatePressureForce(Particle* samplePoint);
        static Vector2 CalculateRepelForce(Particle* samplePoint);
   
};

#endif