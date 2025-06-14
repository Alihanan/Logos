// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Geometry/SphereIcosaMeshGenerator.h"

#include <cstdint>
#include "ASphereGridTile.generated.h"

class UProceduralMeshComponent;
//class SphereIcosaMeshGenerator;

UCLASS()
class LOGOS_API ASphereGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphereGridTile();
	~ASphereGridTile() {}

	void parametrize(const FHexGridRenderData& data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UProceduralMeshComponent* proceduralMesh;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> vertices;

	UPROPERTY(EditAnywhere)
	TArray<int> triangles;


	FHexGridRenderData renderData;

	//SphereIcosaMeshGenerator* generatorMesh;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
