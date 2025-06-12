// Fill out your copyright notice in the Description page of Project Settings.


#include "ASphereGrid.h"

#include "ProceduralMeshComponent.h"

#include <cmath>

// Sets default values
AASphereGrid::AASphereGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

}
/*
void AASphereGrid::Destroyed()
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
void AASphereGrid::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty world, skip!\n"));
		return;  // skip in viewport preview
	}

	this->generatorMesh = new SphereIcosaMeshGenerator(this->RADIUS, pow(2, this->NUM_SUBDIVIDE));
	
	FHexGridRenderData data;

	if(id == 0)
		data = generatorMesh->generateHexagon(1, 0, 1);
	if(id == 1)
		data = generatorMesh->generateHexagon(1, 1, 2);
	if (id == 2)
		data = generatorMesh->generateHexagon(1, 1, 2);
	if (id == 3)
		data = generatorMesh->generateHexagon(1, 1, 2);

	this->vertices = data.Vertices;
	this->triangles = data.Triangles;

	if (id == 1)
		this->vertices = data.IcosaVertices;

	if (id == 2)
		this->vertices = data.AllChunkVertices;

	//if(this->vertices.Num() > 0)
		//SetActorLocation(this->vertices[0]);


	if ((id != 1) && (id != 2)) {
		SetActorLocation(FVector(0, 0, 0));

		this->proceduralMesh = NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass(),
			"ProceduralMesh");

		if (this->proceduralMesh == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Error! Can't create subobject!"));
			return;
		}

		this->proceduralMesh->RegisterComponent();
		this->proceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// Enable editor visibility
		this->proceduralMesh->SetFlags(RF_Transactional);
		this->proceduralMesh->Modify();
	
	
		this->proceduralMesh->CreateMeshSection(
			0,          // Section Index
			this->vertices, //data.Vertices,   // Vertices
			this->triangles,  // Triangles
			TArray<FVector>(),    // Normals
			TArray<FVector2D>(),  // UVs
			TArray<FColor>(),     // Vertex Colors
			TArray<FProcMeshTangent>(), // Tangents
			true        // Create Collision
		);
	}
	//this->SetActorLabel("Kal1brov");
}

// Called every frame
void AASphereGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

