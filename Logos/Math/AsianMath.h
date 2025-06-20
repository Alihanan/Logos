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
inline static double access2DimArray(const double* arr, int x, int y)
{

}

template<typename T>
struct TArray2D
{
    int32 Width = 0, Height = 0, PixelNum = 0;
    TArray<T> Data = TArray<T>();

    TArray2D() = default;

    TArray2D(int32 InW, int32 InH)
    {
        Width = InW; Height = InH;
        PixelNum = Width * Height;
        Data.SetNumUninitialized(PixelNum);
    }

    FORCEINLINE T& operator()(int32 X, int32 Y)
    {
        return Data[Y * Width + X];
    }
    FORCEINLINE const T& operator()(int32 X, int32 Y) const { 
        return Data[Y * Width + X]; 
    }
    FORCEINLINE       T& operator[](int32 Index)
    {
        check(Index >= 0 && Index < Data.Num());
        return Data[Index];
    }
    FORCEINLINE const T& operator[](int32 Index) const
    {
        check(Index >= 0 && Index < Data.Num());
        return Data[Index];
    }
    FORCEINLINE       T* GetData() { return Data.IsValidIndex(0) ? Data.GetData() : nullptr; }
    FORCEINLINE const T* GetData() const { return Data.IsValidIndex(0) ? Data.GetData() : nullptr; }
    
    FORCEINLINE T* GetRow(int32 Y)
    {
        check(Y >= 0 && Y < Height);
        return Data.GetData() + Y * Width;
    }
    FORCEINLINE const T* GetRow(int32 Y) const
    {
        check(Y >= 0 && Y < Height);
        return Data.GetData() + Y * Width;
    }

    T SampleBilinear(double U, double V) const
    {
        UE_LOG(LogTemp, Warning, TEXT("SampleBilinear: U: %f | V %f\n"), U, V);
        if (Width == 0 || Height == 0 || Data.Num() == 0)
        {
            return 0.0;
        }

        // Clamp UV to [0,1]
        //const float ClampedU = FMath::Clamp(U, 0.0f, 1.0f);
        //const float ClampedV = FMath::Clamp(V, 0.0f, 1.0f);

        const double WrappedU = FMath::Frac(U);
        const double WrappedV = FMath::Frac(V);


        // Map to continuous pixel space [0 .. Width-1], [0 .. Height-1]
        const double X = WrappedU * (Width - 1);
        const double Y = WrappedV * (Height - 1);

        // Base indices
        const int32 X0 = FMath::FloorToInt(X);
        const int32 Y0 = FMath::FloorToInt(Y);
        const int32 X1 = FMath::Min(X0 + 1, Width - 1);
        const int32 Y1 = FMath::Min(Y0 + 1, Height - 1);

        // Fractional parts
        const double Fx = X - X0;
        const double Fy = Y - Y0;

        // Fetch four samples
        const T H00 = Data[Y0 * Width + X0];
        const T H10 = Data[Y0 * Width + X1];
        const T H01 = Data[Y1 * Width + X0];
        const T H11 = Data[Y1 * Width + X1];

        // Interpolate along X
        const double H0 = FMath::Lerp(H00, H10, Fx);
        const double H1 = FMath::Lerp(H01, H11, Fx);

        // Then along Y
        return FMath::Lerp(H0, H1, Fy);
    }

};