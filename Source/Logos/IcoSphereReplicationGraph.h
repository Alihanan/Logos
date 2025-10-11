// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "IcoSphereReplicationGraph.generated.h"

/**
 *  Setup and gloval turn ON/OFF is taken from:
 *  https://github.com/readyplayerme/Lyra-Sample/blob/b8474915c547d86c25476ff40ab025926e10cf10/Source/LyraGame/System/LyraReplicationGraph.cpp#L135
 * 
 *  Tutorial used for implementation (RU)
 *  https://www.youtube.com/watch?v=9gpvENkunY4:
 *  
 */
UCLASS()
class LOGOS_API UIcoSphereReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()

public:
	UIcoSphereReplicationGraph();

private:
	UIcoSphereReplicationGraphConnection* GetConnectionForActor(const AActor* Actor);

protected:
	virtual void InitGlobalGraphNodes() override;

	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager) override;

	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo) override;

private:
	UReplicationGraphNode_AlwaysRelevant* AlwaysRelevant = nullptr;
};

UCLASS()
class LOGOS_API UIcoSphereReplicationGraphNode_TileRelevancy : public UReplicationGraphNode_ActorList
{
	GENERATED_BODY()

};

UCLASS()
class LOGOS_API UIcoSphereReplicationGraphConnection : public UNetReplicationGraphConnection
{
	GENERATED_BODY()

	friend UIcoSphereReplicationGraph;

private:
	UIcoSphereReplicationGraphNode_TileRelevancy* tileRelevant = nullptr;

	UReplicationGraphNode_AlwaysRelevant_ForConnection* alwaysRelevantForConn = nullptr;

	uint8 bTileRelevant = 1;

};