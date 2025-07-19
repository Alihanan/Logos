// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "IcoSphereReplicationGraph.generated.h"

/**
 * 
 */
UCLASS()
class LOGOS_API UIcoSphereReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()

public:
	UIcoSphereReplicationGraph();

protected:
	virtual void InitGlobalGraphNodes() override;

private:
	UReplicationGraphNode_AlwaysRelevant* AlwaysRelevant = nullptr;
};
