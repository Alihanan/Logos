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
	void setMaterial(UMaterialInterface* material);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void initialize();

protected:
	UProceduralMeshComponent* proceduralMesh;

	UMaterialInterface* material = nullptr;
	FHexGridRenderData renderData;

	//UPROPERTY(EditAnywhere)
	//TArray<FVector2D> UVs;

	//UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	//TArray<FVector> Vertices;



	//SphereIcosaMeshGenerator* generatorMesh;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
