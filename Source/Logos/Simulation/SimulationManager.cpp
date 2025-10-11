// Fill out your copyright notice in the Description page of Project Settings.


#include "Simulation/SimulationManager.h"

SimulationManager::SimulationManager(uint32_t N_divisions, double RADIUS) :
	N_divisions(N_divisions), RADIUS(RADIUS)
{
	this->simulationData = generate_simulation_struct(this->N_divisions, this->RADIUS);
}

SimulationManager::~SimulationManager()
{
}
