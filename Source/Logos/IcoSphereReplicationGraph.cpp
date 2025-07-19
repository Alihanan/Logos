// Fill out your copyright notice in the Description page of Project Settings.


#include "IcoSphereReplicationGraph.h"

UIcoSphereReplicationGraph::UIcoSphereReplicationGraph()
{
	if (!UReplicationDriver::CreateReplicationDriverDelegate().IsBound())
	{
		UReplicationDriver::CreateReplicationDriverDelegate().BindLambda(
			[](UNetDriver* ForNetDriver, const FURL& URL, UWorld* World) -> UReplicationDriver*
			{
				// Only create for GameNetDriver
				if (World && ForNetDriver && ForNetDriver->NetDriverName == NAME_GameNetDriver)
				{
					UE_LOG(LogTemp, Warning, TEXT("Successfully Initialized IcoSphere Replication Graph"));
					return NewObject<UIcoSphereReplicationGraph>(GetTransientPackage());
				}
				UE_LOG(LogTemp, Warning, TEXT("Error! Using default Replication Graph!"));
				return nullptr;
			});
	}
}

void UIcoSphereReplicationGraph::InitGlobalGraphNodes()
{
	Super::InitGlobalGraphNodes();

	UE_LOG(LogTemp, Warning, TEXT("Global Nodes initialized in IcoSphere Replication Graph!"));
	this->AlwaysRelevant = this->CreateNewNode<UReplicationGraphNode_AlwaysRelevant>();

	AddGlobalGraphNode(this->AlwaysRelevant);
}
