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

	void parametrize(FIcosaPointCoord tileCoord);
	void setMaterial(UMaterialInterface* material);

	virtual bool IsNetRelevantFor
	(
		const AActor* RealViewer,
		const AActor* ViewTarget,
		const FVector& SrcLocation
	) const override;


	UPROPERTY(ReplicatedUsing = OnRep_GenerateHexagon)
	FIcosaPointCoord tileIcosaPoint = FIcosaPointCoord(EIcosID::ID_EMPTY, 0, 0, 4);

	UFUNCTION()
	void OnRep_GenerateHexagon();

	TArray<FIcosaPointCoord> releventChunkPositions;

	SphereIcosaMeshGenerator* generatorMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateProceduralComponent();

	virtual void OnRep_Owner() override;

	void ExtractParametersFromOwner();

	void ReGenerateLocalComponentAndHexagon();
	void DestroyLocalComponentAndHexagon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UProceduralMeshComponent* proceduralMesh;

	UMaterialInterface* material = nullptr;
	FHexGridRenderData renderData;

	UPROPERTY(EditAnywhere)
	TArray<FVector2D> UVs;

	UPROPERTY(EditAnywhere)
	TArray<int32> Triangles;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> Vertices;



	//SphereIcosaMeshGenerator* generatorMesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
