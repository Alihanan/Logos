// Fill out your copyright notice in the Description page of Project Settings.



#include "Geometry/ASphereGridController.h"
// Sets default values
AASphereGridController::AASphereGridController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASphereGridController::BeginPlay()
{
	Super::BeginPlay();

	
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty world, skip!\n"));
		return;  // skip in viewport preview
	}

	UWorld* world = GetWorld();
	int32 N_divisions = pow(2, this->NUM_SUBDIVIDE);
	this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, N_divisions);

	for (int f = 1; f < 11; f++) 
	{
		//if ((f != 6) && (f != 1)) continue;
		for (int x = 0; x < N_divisions; x++)
		{
			for (int y = 0; y < N_divisions; y++)
			{
				ASphereGridTile* NewActor = world->SpawnActor<ASphereGridTile>(ASphereGridTile::StaticClass(),
					GetActorLocation(),
					GetActorRotation()
				);
				if (NewActor)
				{
					FHexGridRenderData data = this->generatorMesh->generateHexagon(f, x, y);
					NewActor->setMaterial(this->material);
					NewActor->parametrize(data);
				}
				
			}
		}
	}
	ASphereGridTile* NewActorNorthPole = world->SpawnActor<ASphereGridTile>(ASphereGridTile::StaticClass(),
		GetActorLocation(),
		GetActorRotation()
	);
	if (NewActorNorthPole)
	{
		FHexGridRenderData data = this->generatorMesh->generateHexagon(0, 0, 0);
		NewActorNorthPole->parametrize(data);
		NewActorNorthPole->setMaterial(this->material);
	}

	ASphereGridTile* NewActorSouthPole = world->SpawnActor<ASphereGridTile>(ASphereGridTile::StaticClass(),
		GetActorLocation(),
		GetActorRotation()
	);
	if (NewActorSouthPole)
	{
		FHexGridRenderData data = this->generatorMesh->generateHexagon(11, 0, 0);
		NewActorSouthPole->parametrize(data);
		NewActorSouthPole->setMaterial(this->material);
	}
}

// Called every frame
void AASphereGridController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

