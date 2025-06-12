// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Geometry/SphereIcosaMeshGenerator.h"

#include <cstdint>
#include "ASphereGrid.generated.h"

class UProceduralMeshComponent;
//class SphereIcosaMeshGenerator;

UCLASS()
class LOGOS_API AASphereGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASphereGrid();
	~AASphereGrid() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UProceduralMeshComponent* proceduralMesh;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> vertices;

	UPROPERTY(EditAnywhere)
	TArray<int> triangles;

	SphereIcosaMeshGenerator* generatorMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 NUM_SUBDIVIDE = 2;

	UPROPERTY(EditAnywhere)
	double RADIUS = 1000.0;

	UPROPERTY(EditAnywhere)
	int32 id = 0;

};
