// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubscriberSimulation.h"

#include <list>
#include <stdint.h>
#include "LenpartyaSimulationCUDA/icosphere.h"


/**
 * This class implements Publisher part of Publish-Subscribe pattern
 * 
 * We will run simulation of states and propagate the state to 
 */
class LOGOS_API SimulationManager
{
public:

	uint32_t N_divisions = 0;
	double RADIUS = 0;

	SimulationManager() :
		N_divisions(0), RADIUS(0.0) {}

	SimulationManager(uint32_t N_divisions, double RADIUS);
	~SimulationManager();

	SimulationData* simulationData = nullptr;


	std::list<ISubscriberSimulation*> subscribers;
	
	void addSubscriber(ISubscriberSimulation* toAddSub)
	{
		this->subscribers.push_back(toAddSub);
		toAddSub->receiveUpdate(this->simulationData);
	}

	void removeSubscriber(ISubscriberSimulation* toDeleteSub)
	{
		this->subscribers.remove(toDeleteSub);
	}

	void notifyAll()
	{
		for (auto it = subscribers.begin(); it != subscribers.end(); )
		{
			auto current = it++;
			if (*current)
				(*current)->receiveUpdate(this->simulationData);
		}
	}
};
