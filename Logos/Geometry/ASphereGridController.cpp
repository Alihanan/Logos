// Fill out your copyright notice in the Description page of Project Settings.


#if WITH_EDITOR
#include "Editor.h"
#include "EditorViewportClient.h"
#endif
#include "Geometry/ASphereGridController.h"
#include <sstream>
#include <set>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/SpectatorPawn.h"

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
	/*
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("SpectatorPawn");
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLoc = FVector(2000, 0, 0);
	FRotator SpawnRot = FRotator::ZeroRotator;

	ASpectatorPawn* SpecPawn = GetWorld()->SpawnActor<ASpectatorPawn>(
		ASpectatorPawn::StaticClass(),
		SpawnLoc,
		SpawnRot,
		SpawnParams
	);*/

	//this->player = GetWorld()->GetFirstPlayerController()->GetPawn();
	//this->camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//this->camera = UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager;

	InitializeHeightmap();

	UWorld* world = GetWorld();
	this->N_divisions = pow(2, this->NUM_SUBDIVIDE);
	//this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, N_divisions, );
	this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, N_divisions, &(this->heightmapData), HEIGHT_ABOVE_RADIUS);
	
	/*
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
				activeChunks[FIcosaPointCoord(f, x, y, N_divisions)] = NewActor;
				//actorTiles.Add(NewActor);
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
	activeChunks[FIcosaPointCoord(0, 0, 0, N_divisions)] = NewActorNorthPole;
	//actorTiles.Add(NewActorNorthPole);

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
	activeChunks[FIcosaPointCoord(11, 0, 0, N_divisions)] = NewActorSouthPole;
	//actorTiles.Add(NewActorSouthPole);
	*/
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


static FVector GetCameraLocation(UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return FVector::ZeroVector;
	}

#if WITH_EDITOR
	// If we're in the editor and not running a game world, grab the editor viewport camera
	if (GIsEditor && !WorldContext->IsGameWorld() && GEditor)
	{
		for (FEditorViewportClient* VC : GEditor->GetAllViewportClients())
		{
			// Find the focused perspective viewport
			if (VC && VC->IsPerspective() && VC->Viewport && VC->Viewport->HasFocus())
			{
				return VC->GetViewLocation();
			}
		}
	}
#endif

	// Otherwise, assume we have a real PlayerController + CameraManager
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, 0))
	{
		if (PC->PlayerCameraManager)
		{
			return PC->PlayerCameraManager->GetCameraLocation();
		}
	}

	return FVector::ZeroVector;
}

void AASphereGridController::UpdateChunks(std::set<FIcosaPointCoord>& toAdd)
{
	auto RemoveChunk = [&](const FIcosaPointCoord& C) {
		auto val = activeChunks[C];
		val->Destroy();
		};

	auto AddChunk = [&](const FIcosaPointCoord& C) {
		ASphereGridTile* NewActor = GetWorld()->SpawnActor<ASphereGridTile>(ASphereGridTile::StaticClass(),
			GetActorLocation(),
			GetActorRotation()
		);
		if (NewActor)
		{
			FHexGridRenderData data = this->generatorMesh->generateHexagon(C);
			NewActor->setMaterial(this->material);
			NewActor->parametrize(data);
		}
		return NewActor;
		};


	auto itA = this->activeChunks.begin();
	auto itD = toAdd.begin();


	while ((itA != this->activeChunks.end()) && (itD != toAdd.end()))
	{
		const FIcosaPointCoord& aKey = itA->first;
		const FIcosaPointCoord& dKey = *itD;

		if (aKey < dKey)
		{
			itA->second->Destroy();
			//itA->second->SetLifeSpan(0.1);
			//RemoveChunk(aKey);
			// erase at that position, get the next iterator
			itA = this->activeChunks.erase(itA);
		}
		else if (dKey < aKey)
		{
			auto newVal = AddChunk(dKey);
			this->activeChunks.emplace_hint(itA, dKey, newVal);
			++itD;
		}
		else {
			++itA;
			++itD;
		}
	}

	while (itA != this->activeChunks.end())
	{
		const FIcosaPointCoord& aKey = itA->first;
		itA->second->Destroy();
		//itA->second->SetLifeSpan(0.1);
		//RemoveChunk(aKey);
		itA = this->activeChunks.erase(itA);
	}

	while (itD != toAdd.end())
	{
		const FIcosaPointCoord& dKey = *itD;
		auto newVal = AddChunk(dKey);
		this->activeChunks.emplace_hint(itA, dKey, newVal);
		++itD;
	}
}


// Called every frame
void AASphereGridController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	this->totalTime += DeltaTime;

	//if (totalTime < 10.0) {
	//	return;
	//}
	//totalTime -= 10.0;

	//FVector playerLoc = this->player->GetActorLocation();
	
	FVector playerLoc = GetCameraLocation(GetWorld());
	auto point = FIcosaPointCoord::convertPositionToIcosaPoint(playerLoc, this->N_divisions);

	//auto data = this->generatorMesh->generateHexagonRadius(point.coord.id, point.X, point.Y, this->RADIUS_CHUNK);



	

	if (prevKey == point) {
		return;
	}

	GEngine->ClearOnScreenDebugMessages();

	std::ostringstream out;
	out << "Player 3D: [" << playerLoc.X << "," << playerLoc.Y << "," << playerLoc.Z << "]";
	FString txt = UTF8_TO_TCHAR(out.str().c_str());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, txt);

	out << "Player 2D Icosa: [" << static_cast<int>(point.coord.id) << "," << point.X << "," << point.Y << "]";
	txt = UTF8_TO_TCHAR(out.str().c_str());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, txt);

	prevKey = point;

	std::set<FIcosaPointCoord> toAddKeys = FIcosaPoint(point).getNeighbourRadius(this->RADIUS_CHUNK);

	this->UpdateChunks(toAddKeys);
}

