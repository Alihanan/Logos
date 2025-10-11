#pragma once


#include "LenpartyaSimulationCUDA/icosphere.h"

#include "UObject/Interface.h"
#include "SubscriberSimulation.generated.h"

UINTERFACE(BlueprintType)
class USubscriberSimulation : public UInterface
{
	GENERATED_BODY()
};


class ISubscriberSimulation
{
	GENERATED_BODY()
public:
	virtual void receiveUpdate(const SimulationData* dataState) = 0;
};