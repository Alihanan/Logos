// Fill out your copyright notice in the Description page of Project Settings.


#include "ASphereGridTile.h"

#include "ProceduralMeshComponent.h"

#include <cmath>

// Sets default values
ASphereGridTile::ASphereGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

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
	this->initialize();
}

void ASphereGridTile::initialize()
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
}


void ASphereGridTile::parametrize(const FHexGridRenderData& data)
{
	this->initialize();
	this->renderData = data;

	this->UVs = data.UVs;
	this->Vertices = data.Vertices;

	SetActorLabel(data.name);
	SetActorLocation(FVector(0, 0, 0));

	// Enable editor visibility
	
	this->proceduralMesh->CreateMeshSection(
		0,          // Section Index
		renderData.Vertices, //data.Vertices,   // Vertices
		renderData.Triangles,  // Triangles
		renderData.Normals,    // Normals
		renderData.UVs,  // UVs
		renderData.colors,     // Vertex Colors
		TArray<FProcMeshTangent>(), // Tangents
		true        // Create Collision
	);
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

