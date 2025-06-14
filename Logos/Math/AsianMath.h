#pragma once


#define A_PI 3.141592653589793
#define A_PHI_GLD 1.618033988749895

#define A_ONE_OVER_SQRT5 0.44721359549995793928183473374625524708812367192230514485417944908210418512

#include "CoreMinimal.h"

inline static FVector2D transformSpherical(const FVector& xyz)
{
    FVector N = xyz.GetSafeNormal();
    float Theta = FMath::Acos(N.Z);                
    float Phi = FMath::Atan2(N.Y, N.X);          
    return FVector2D(Theta, Phi);
}
inline static FVector2D transformSphericalUV(const FVector& xyz)
{
    FVector2D uv_neigh = transformSpherical(xyz);
    FVector2D ret;
    ret.Y = uv_neigh.X / A_PI;
    ret.X = uv_neigh.Y / (-2.0 * A_PI) + 0.5;
    return ret;
}