// Fill out your copyright notice in the Description page of Project Settings.


#include "ASphereGridTile.h"

#include "ProceduralMeshComponent.h"
#include <cmath>
#include <Net/UnrealNetwork.h>
#include <ASphereGridController.h>

// Sets default values
ASphereGridTile::ASphereGridTile()
{
	UE_LOG(LogTemp, Warning, TEXT("ASphereGRidTile created!\n"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	this->SetReplicates(true);
	this->SetReplicateMovement(true);

	this->bReplicates = true;
	this->bNetLoadOnClient = true;

	auto owner = this->GetOwner();

	if (auto ownerChunkController = Cast<AASphereGridController>(owner))
	{
		this->generatorMesh = ownerChunkController->generatorMesh;
	}
}
/*
void ASphereGridTile::Destroyed()
{
	if (IsValid(UProceduralMeshComponent))
	{
		Actor->Destroy();
	}

	SpawnedActors.Empty();

	Super::Destroyed();
}
*/
bool ASphereGridTile::IsNetRelevantFor
(
	const AActor* RealViewer,
	const AActor* ViewTarget,
	const FVector& SrcLocation
) const
{
	//return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	/*if (bAlwaysRelevant || IsOwnedBy(ViewTarget) || IsOwnedBy(RealViewer) || this == ViewTarget || ViewTarget == GetInstigator())
	{
		return true;
	}
	else if (bNetUseOwnerRelevancy && Owner)
	{
		return Owner->IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}
	else if (bOnlyRelevantToOwner)
	{
		return false;
	}
	else if (RootComponent && RootComponent->GetAttachParent() && RootComponent->GetAttachParent()->GetOwner() && (Cast<USkeletalMeshComponent>(RootComponent->GetAttachParent()) || (RootComponent->GetAttachParent()->GetOwner() == Owner)))
	{
		return RootComponent->GetAttachParent()->GetOwner()->IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}
	else if (IsHidden() && (!RootComponent || !RootComponent->IsCollisionEnabled()))
	{
		return false;
	}

	if (!RootComponent)
	{
		UE_LOG(LogNet, Warning, TEXT("Actor %s / %s has no root component in AActor::IsNetRelevantFor. (Make bAlwaysRelevant=true?)"), *GetClass()->GetName(), *GetName());
		return false;
	}

	return !GetDefault<AGameNetworkManager>()->bUseDistanceBasedRelevancy ||
		IsWithinNetRelevancyDistance(SrcLocation);
	*/
	
	if (const APlayerController* Player = Cast<APlayerController>(RealViewer))
	{
		// Without this client wont be able to generate hex tiles!
		if (this->HasAuthority())
		{
			return true; // Server always has this thing replicated?
		}

		FVector playerLoc = RealViewer->GetActorLocation();
		auto point = FIcosaPointCoord::convertPositionToIcosaPoint(playerLoc, this->tileIcosaPoint.N_division);

		for (auto pos : this->releventChunkPositions)
		{
			if (point == pos)
			{
				return true;
			}
		}
	}
	return false; 
}

// Called when the game starts or when spawned
void ASphereGridTile::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty world, skip!\n"));
		return;  // skip in viewport preview
	}

	this->ExtractParametersFromOwner();
}

void ASphereGridTile::CreateProceduralComponent()
{
	if (this->proceduralMesh != nullptr) return;
	this->proceduralMesh = NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass(),
		"ProceduralMesh");

	if (this->proceduralMesh == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Error! Can't create subobject!"));
		return;
	}
	
	this->proceduralMesh->RegisterComponent();
	this->proceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	this->proceduralMesh->SetFlags(RF_Transactional);
	this->proceduralMesh->Modify();
	

	this->proceduralMesh->SetNetAddressable();

	this->proceduralMesh->SetIsReplicated(false);
}

void ASphereGridTile::OnRep_Owner()
{
	Super::OnRep_Owner();
	this->ExtractParametersFromOwner();
}

void ASphereGridTile::ExtractParametersFromOwner()
{
	auto owner = this->GetOwner();

	if (auto ownerChunkController = Cast<AASphereGridController>(owner))
	{
		this->generatorMesh = ownerChunkController->generatorMesh;
		//this->material = ownerChunkController->material;
		this->CreateProceduralComponent();
		this->setMaterial(ownerChunkController->material);
	}
}


void ASphereGridTile::parametrize(FIcosaPointCoord tileCoord)
{
	this->tileIcosaPoint = tileCoord;
	OnRep_GenerateHexagon();
}
void ASphereGridTile::setMaterial(UMaterialInterface* mat)
{
	if (mat == nullptr) return;
	this->material = mat;
	this->proceduralMesh->SetMaterial(0, this->material);
}

// Called every frame
void ASphereGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASphereGridTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASphereGridTile, tileIcosaPoint);
}

void ASphereGridTile::OnRep_GenerateHexagon()
{
	UE_LOG(LogTemp, Warning, TEXT("Chunk [%d %d %d] has OnRep called!\n"), this->tileIcosaPoint.coord.id, this->tileIcosaPoint.X, this->tileIcosaPoint.Y);

	this->tileIcosaPoint.correctAfterReplication();

	UE_LOG(LogTemp, Warning, TEXT("Chunk [%d %d %d] has OnRep called!\n"), this->tileIcosaPoint.coord.id, this->tileIcosaPoint.X, this->tileIcosaPoint.Y);

	bool isServer = this->HasAuthority();
	if (isServer == false) {
		int asd = 1;
	}

	auto owner = this->GetOwner();
	if (this->generatorMesh == nullptr)
	{
		int asd = 1;
	}


	this->releventChunkPositions.Add(this->tileIcosaPoint);
	this->releventChunkPositions.Append(this->tileIcosaPoint.getNeighbours());

	FHexGridRenderData data = this->generatorMesh->generateHexagon(this->tileIcosaPoint);

	
	this->UVs = data.UVs;
	this->Vertices = data.Vertices;
	this->Triangles = data.Triangles;

#if WITH_EDITOR
	this->SetActorLabel(data.name);
#endif
	SetActorLocation(FVector(0, 0, 0));

	// Enable editor visibility
	this->proceduralMesh->CreateMeshSection(
		0,          // Section Index
		data.Vertices, //data.Vertices,   // Vertices
		data.Triangles,  // Triangles
		data.Normals,    // Normals
		data.UVs,  // UVs
		data.colors,     // Vertex Colors
		TArray<FProcMeshTangent>(), // Tangents
		true        // Create Collision
	);
}