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

	InitializeHeightmap();

	UWorld* world = GetWorld();
	int32 N_divisions = pow(2, this->NUM_SUBDIVIDE);
	//this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, N_divisions, );
	this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, N_divisions, &(this->heightmapData), HEIGHT_ABOVE_RADIUS);


	for (int f = 2; f < 3; f++) 
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



void AASphereGridController::InitializeHeightmap()
{
	if (this->heightmapTexture == nullptr) return;

	
	FTexture2DMipMap& Mip = heightmapTexture->GetPlatformData()->Mips[0];
	EPixelFormat fmt = heightmapTexture->GetPixelFormat();
	void* RawData = Mip.BulkData.Lock(LOCK_READ_ONLY);
	// … copy out your pixels …

	int width = Mip.SizeX;
	int height = Mip.SizeY;
	int N = width * height;

	// Allocate array
	heightmapData = TArray2D<double>(width, height);
	double* RawOut = heightmapData.GetData();

	if (fmt == EPixelFormat::PF_B8G8R8A8)
	{
		const FColor* Colors = static_cast<const FColor*>(RawData);
		for (int i = 0; i < N; i++) {
			RawOut[i] = Colors[i].R / 255.0;
		}
	}
	else if (fmt == EPixelFormat::PF_FloatRGBA)
	{
		const FFloat16Color* FPix = static_cast<const FFloat16Color*>(RawData);
		for (int i = 0; i < N; i++) {
			RawOut[i] = FPix[i].R;
		}
	}
	else if (fmt == EPixelFormat::PF_G8)
	{
		const uint8* Grays = static_cast<const uint8*>(RawData);
		for (int i = 0; i < N; i++) {
			RawOut[i] = Grays[i] / 255.0;
		}
	}
	else
	{
		// TODO make error
	}

	Mip.BulkData.Unlock();
}

// Called every frame
void AASphereGridController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

