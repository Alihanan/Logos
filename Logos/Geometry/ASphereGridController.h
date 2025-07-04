// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/AsianMath.h"
#include "Geometry/ASphereGridTile.h"
#include "Geometry/SphereIcosaMeshGenerator.h"

#include <queue>
#include <map>
#include <set>

#include "ASphereGridController.generated.h"


UCLASS()
class LOGOS_API AASphereGridController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASphereGridController();

protected:
	SphereIcosaMeshGenerator* generatorMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeHeightmap();

	UPROPERTY(EditAnywhere)
	int32 NUM_SUBDIVIDE = 2;

	int N_divisions = 4;

	UPROPERTY(EditAnywhere)
	double RADIUS = 1000.0;

	UPROPERTY(EditAnywhere)
	double HEIGHT_ABOVE_RADIUS = 100.0;

	UPROPERTY(EditAnywhere)
	int32 RADIUS_CHUNK = 3;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;

	UPROPERTY(EditAnywhere)
	UTexture2D* heightmapTexture = nullptr;


	UPROPERTY(EditAnywhere)
	FVector currentPlayerLocation;

	TArray2D<double> heightmapData;
	
protected:
	//TArray<ASphereGridTile*> actorTiles;
	APawn* player;
	APlayerCameraManager* camera;

	FIcosaPointCoord prevKey = FIcosaPointCoord(0, 0, 0, 0);


	///std::unordered_map<FIcosaPointCoord, ASphereGridTile*, FIcosaPointCoord::Hasher> activeChunks;
	std::map<FIcosaPointCoord, ASphereGridTile*> activeChunks;

	void UpdateChunks(std::set<FIcosaPointCoord>& toAdd);


	struct FIcosaPointCoordChunkQueue
	{
		//std::unordered_map<FIcosaPointCoord, ASphereGridTile*, FIcosaPointCoord::Hasher> actorTiles;

		//std::unordered_map<FIcosaPointCoord, ASphereGridTile*, FIcosaPointCoord::Hasher> activeChunks;

		//std::queue<FIcosaPointCoord> key;

		//std::vector<FIcosaPointCoord> key;






	};



	double totalTime = 0.0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




};
