// Fill out your copyright notice in the Description page of Project Settings.


#include "IcoSphereReplicationGraph.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

UIcoSphereReplicationGraph::UIcoSphereReplicationGraph()
{
	if (!UReplicationDriver::CreateReplicationDriverDelegate().IsBound())
	{
		UReplicationDriver::CreateReplicationDriverDelegate().BindLambda(
			[](UNetDriver* ForNetDriver, const FURL& URL, UWorld* World) -> UReplicationDriver*
			{
				/*
				TODO: remove after you finalize Replication Graph
				*/
				UE_LOG(LogTemp, Warning, TEXT("DEBUG! Using default Replication Graph for now...!"));
				return nullptr;
				/*
				// Only create for GameNetDriver
				if (World && ForNetDriver && ForNetDriver->NetDriverName == NAME_GameNetDriver)
				{
					UE_LOG(LogTemp, Warning, TEXT("Successfully Initialized IcoSphere Replication Graph"));
					return NewObject<UIcoSphereReplicationGraph>(GetTransientPackage());
				}
				UE_LOG(LogTemp, Warning, TEXT("Error! Using default Replication Graph!"));
				return nullptr;
				*/
			});
	}

	// Change class of connection to our
	ReplicationConnectionManagerClass = UIcoSphereReplicationGraphConnection::StaticClass();
}

UIcoSphereReplicationGraphConnection* UIcoSphereReplicationGraph::GetConnectionForActor(const AActor* Actor)
{
	if (Actor) {
		UNetConnection* connection = Actor->GetNetConnection();
		if (connection) {

			// Guaranteed to be non-nullptr
			UNetReplicationGraphConnection* connManager = FindOrAddConnectionManager(connection);

			UIcoSphereReplicationGraphConnection* graphConn = Cast<UIcoSphereReplicationGraphConnection>(connManager);

			if (graphConn) {
				return graphConn;
			}
		}
	}
	return nullptr;
}

void UIcoSphereReplicationGraph::InitGlobalGraphNodes()
{
	Super::InitGlobalGraphNodes();

	UE_LOG(LogTemp, Warning, TEXT("Global Nodes initialized in IcoSphere Replication Graph!"));
	this->AlwaysRelevant = this->CreateNewNode<UReplicationGraphNode_AlwaysRelevant>();

	AddGlobalGraphNode(this->AlwaysRelevant);
}

void UIcoSphereReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager)
{
	Super::InitConnectionGraphNodes(ConnectionManager);

	UIcoSphereReplicationGraphConnection* graphConn = Cast<UIcoSphereReplicationGraphConnection>(ConnectionManager);

	if (graphConn) 
	{
		graphConn->alwaysRelevantForConn = this->CreateNewNode<UReplicationGraphNode_AlwaysRelevant_ForConnection>();

		AddConnectionGraphNode(graphConn->alwaysRelevantForConn, graphConn);

		graphConn->tileRelevant = this->CreateNewNode<UIcoSphereReplicationGraphNode_TileRelevancy>();

		AddConnectionGraphNode(graphConn->tileRelevant, graphConn);
	}
}

void UIcoSphereReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo)
{
	Super::RouteAddNetworkActorToNodes(ActorInfo, GlobalInfo);

	if (ActorInfo.Class->IsChildOf(AGameStateBase::StaticClass()) ||
		ActorInfo.Class->IsChildOf(APlayerState::StaticClass()))
	{
		AlwaysRelevant->NotifyAddNetworkActor(ActorInfo);
	}


	UIcoSphereReplicationGraphConnection* connActor = GetConnectionForActor(ActorInfo.Actor);

	if (connActor && ActorInfo.Actor->bOnlyRelevantToOwner) {
		connActor->alwaysRelevantForConn->NotifyAddNetworkActor(ActorInfo);
	}
}
