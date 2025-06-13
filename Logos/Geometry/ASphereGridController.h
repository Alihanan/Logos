// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Geometry/ASphereGridTile.h"
#include "Geometry/SphereIcosaMeshGenerator.h"

#include "ASphereGridController.generated.h"


UCLASS()
class LOGOS_API AASphereGridController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASphereGridController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 NUM_SUBDIVIDE = 2;

	UPROPERTY(EditAnywhere)
	double RADIUS = 1000.0;

	SphereIcosaMeshGenerator* generatorMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
