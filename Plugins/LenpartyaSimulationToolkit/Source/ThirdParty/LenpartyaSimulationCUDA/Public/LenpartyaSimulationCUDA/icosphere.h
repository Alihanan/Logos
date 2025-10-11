#pragma once


#ifndef LENPARTYACUDA_EXPORT

#if defined(LENPARTYACUDA_BUILD) // set this when building the DLL
#if defined(_WIN32) || defined(_WIN64)
#define LENPARTYACUDA_EXPORT __declspec(dllexport)
#elif defined(__linux__)
#define LENPARTYACUDA_EXPORT __attribute__((visibility("default")))
#else
#define LENPARTYACUDA_EXPORT
#endif

#else // consumer side: import the symbol
#if defined(_WIN32) || defined(_WIN64)
#define LENPARTYACUDA_EXPORT __declspec(dllimport)
#elif defined(__linux__)
#define LENPARTYACUDA_EXPORT __attribute__((visibility("default")))
#else
#define LENPARTYACUDA_EXPORT
#endif

#endif // LENPARTYACUDA_BUILD

#endif // LENPARTYACUDA_EXPORT

#if defined _WIN32 || defined _WIN64
#include <Windows.h>

#define LENPARTYACUDA_EXPORT __declspec(dllexport)
#else
#include <stdio.h>
#endif


#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct Coord3D
{
	double X;
	double Y;
	double Z;
};

template<typename T>
struct SGridData
{
	/*
	* Spherical Grid Data

	Note: T* pointer constains a neighbourhood around the main square tile

	For 2x2 tile it will look like:

	X X X X
	X O O X
	X O O X
	X X X X

	Poles are tiles of size 1x1:

	X X X
	X O X
	X X X

	*/
	T* POLE_NORTH = nullptr;
	T* TILE_SQUAD_ARRAYS[10] = {nullptr};
	T* POLE_SOUTH = nullptr;

	uint32_t WIDTH_QUAD = 0;

	SGridData() = default; // Empty constructor

	SGridData(uint32_t WIDTH_QUAD)
	{
		this->WIDTH_QUAD = WIDTH_QUAD;

		POLE_NORTH = new T[9]; // size 1x1
		for (int i = 0; i < 9; i++) POLE_NORTH[i] = { 0.0,0.0,0.0 };


		//memset(POLE_NORTH, 0, sizeof(T) * 9);

		for (int i = 0; i < 10; i++)
		{
			// Quad + surroundings
			TILE_SQUAD_ARRAYS[i] = new T[(WIDTH_QUAD + 1) * (WIDTH_QUAD +1)];

			for (int j = 0; j < (WIDTH_QUAD + 1) * (WIDTH_QUAD + 1); j++) {
				TILE_SQUAD_ARRAYS[i][j] = { 0.0,0.0,0.0 };
			}
			//memset(TILE_SQUAD_ARRAYS[i], 0, sizeof(T) * (WIDTH_QUAD + 1) * (WIDTH_QUAD + 1));
		}
		POLE_SOUTH = new T[9]; // size 1x1
		for (int i = 0; i < 9; i++) POLE_SOUTH[i] = { 0.0,0.0,0.0 };
		//memset(POLE_SOUTH, 0, sizeof(T) * 9);
	}

	~SGridData()
	{
		delete[] POLE_NORTH;
		for (int i = 0; i < 10; i++)
		{
			delete[] TILE_SQUAD_ARRAYS[i];
		}
		delete[] POLE_SOUTH;
	}

};

struct SimulationData
{
	SGridData<Coord3D> coordinate_3d;
	SGridData<Coord3D> coordinate_3d_GPU;

	double RADIUS_PLANET = 0;

	uint32_t WIDTH_QUAD = 0;

	SimulationData() = default;

	SimulationData(uint32_t WIDTH_QUAD, double RADIUS)
	{
		this->RADIUS_PLANET = RADIUS;
		this->WIDTH_QUAD = WIDTH_QUAD;

#if defined _WIN32 || defined _WIN64
		MessageBox(NULL, TEXT("WIDTH_QUAD!"), TEXT("Third Party Plugin"), MB_OK);
#else
		printf("Loaded LenpartyaCUDA manually!");
#endif
		printf("WIDTH_QUAD: %d\n", this->WIDTH_QUAD);

		coordinate_3d = SGridData<Coord3D>(this->WIDTH_QUAD);
	}
};


LENPARTYACUDA_EXPORT void compute_3d_coordinate(SimulationData* data);
LENPARTYACUDA_EXPORT SimulationData* generate_simulation_struct(
	uint32_t NUM_DIVISIONS, double RADIUS
);