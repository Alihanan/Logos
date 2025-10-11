// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/AsianMath.h"
#include "Geometry/ASphereGridTile.h"
#include "Geometry/SphereIcosaMeshGenerator.h"
#include "Net/Serialization/FastArraySerializer.h"
#include <LogosCharacter.h>

#include <queue>
#include <map>
#include <set>

#include "Simulation/SimulationManager.h"


#include "ASphereGridController.generated.h"




UCLASS()
class LOGOS_API AASphereGridController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASphereGridController();

	SphereIcosaMeshGenerator* generatorMesh;
	SimulationManager* simManager;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeHeightmap();

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	int32 NUM_SUBDIVIDE = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SphericalIcosahedron", BlueprintGetter = GetPowerOfTwo)
	int N_divisions = 4;

	UFUNCTION(BlueprintGetter)
	int32 GetPowerOfTwo() const
	{
		return FMath::RoundToInt(FMath::Pow(2.0f, NUM_SUBDIVIDE));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	double RADIUS = 1000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	double HEIGHT_ABOVE_RADIUS = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	int32 RADIUS_CHUNK = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	UMaterialInterface* material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphericalIcosahedron")
	UTexture2D* heightmapTexture = nullptr;


	UPROPERTY(EditAnywhere)
	FVector currentPlayerLocation;

	TArray2D<double> heightmapData;
	
protected:
	//TArray<ASphereGridTile*> actorTiles;
	APawn* player;
	APlayerCameraManager* camera;

	FIcosaPointCoord prevKey = FIcosaPointCoord(EIcosID::ID_EMPTY, 0, 0, 0);


	///std::unordered_map<FIcosaPointCoord, ASphereGridTile*, FIcosaPointCoord::Hasher> activeChunks;
	std::map<FIcosaPointCoord, ASphereGridTile*> activeChunks;

	struct ChunkTrackingSystem
	{
		struct PlayerInfo
		{
			FVector previousLocation;
			FIcosaPointCoord previousIcosaCoord;

			FString playerName;
			bool onServer;
		};

		struct ChunkInfo
		{
			ASphereGridTile* tileObject;
			uint32_t playersInRadius = 0;
		};


		std::unordered_map<FIcosaPointCoord, uint32_t> chunkActivePlayersMap;
		//std::unordered_map<FIcosaPointCoord, ASphereGridTile*, FIcosaPointCoord::Hasher> coordToTileMap;
		

		std::unordered_map<uint32, PlayerInfo*> uniqueIDToPlayerInfoMap;
		std::unordered_map<FIcosaPointCoord, ChunkInfo*> coordToTileMap;

		AASphereGridController* owner;
		int N_divisions = 0;
		

		ChunkTrackingSystem() = default;

		ChunkTrackingSystem(AASphereGridController* owner) :
			owner(owner), N_divisions(owner->N_divisions)
		{}

		

		void EmulatePlayerDisconnect()
		{
			for (auto& keyval : uniqueIDToPlayerInfoMap)
			{
				keyval.second->onServer = false;
			}
			return;
		}

		void MarkSingleChunk(FIcosaPointCoord& chunkCoord)
		{
			ChunkInfo* val;
			if (coordToTileMap.find(chunkCoord) == coordToTileMap.end())
			{
				val = new ChunkInfo;

				val->tileObject = owner->CreateIcosaTile(chunkCoord); // TODO call spawn chunks
				if (val->tileObject == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Somehow the tile cannot be created! [%d | %d %d]"), static_cast<int>(chunkCoord.coord.id), chunkCoord.X, chunkCoord.Y);
					delete val;
					return;
				}

				coordToTileMap[chunkCoord] = val;
			}
			else {
				val = coordToTileMap[chunkCoord];
			}

			val->playersInRadius += 1;
			return;
		}
		void MarkChunkCoordAndNeighbours(FIcosaPointCoord& chunkCoordCenter)
		{
			MarkSingleChunk(chunkCoordCenter);
			auto neighbours = chunkCoordCenter.getNeighbours();
			for (auto& neigh : neighbours)
			{
				MarkSingleChunk(neigh);
			}
			return;
		}
		void DeMarkSingleChunk(FIcosaPointCoord& chunkCoord)
		{
			if (coordToTileMap.find(chunkCoord) == coordToTileMap.end())
			{
				UE_LOG(LogTemp, Error, TEXT("Somehow the chunk activated by player is missing! [%d | %d %d]"), static_cast<int>(chunkCoord.coord.id), chunkCoord.X, chunkCoord.Y);
				return;
			}

			ChunkInfo* val = coordToTileMap[chunkCoord];

			val->playersInRadius -= 1;
			return;
		}
		void DeMarkChunkAndNeighbours(FIcosaPointCoord& chunkCoordCenter)
		{
			DeMarkSingleChunk(chunkCoordCenter);
			auto neighbours = chunkCoordCenter.getNeighbours();
			for (auto& neigh : neighbours)
			{
				DeMarkSingleChunk(neigh);
			}
			return;
		}

		bool CheckOrAddNewPlayer(ALogosCharacter* player)
		{
			auto id = player->GetUniqueID();

			if (uniqueIDToPlayerInfoMap.find(id) != uniqueIDToPlayerInfoMap.end())
				return true;

			FVector loc = player->GetActorLocation();
			auto newIcosaCoord = FIcosaPointCoord::convertPositionToIcosaPoint(loc, this->N_divisions);

			PlayerInfo* val = new PlayerInfo;

			val->previousLocation = loc;
			val->previousIcosaCoord = newIcosaCoord;
			val->onServer = true;

			uniqueIDToPlayerInfoMap[id] = val;

			MarkChunkCoordAndNeighbours(newIcosaCoord);

			return false;
		}

		void ToggleUpdatePlayer(ALogosCharacter* player)
		{
			auto id = player->GetUniqueID();

			FIcosaPointCoord chunkCoord;

			if (CheckOrAddNewPlayer(player) == false)
			{
				return;
			}

			auto previousInfo = uniqueIDToPlayerInfoMap[id];

			previousInfo->onServer = true;  // Confirms that player did not left etc

			FVector newLoc = player->GetActorLocation();

			if (newLoc.Equals(previousInfo->previousLocation))
				return; // No changes if does no move  ---> TODO: not marking again!

			previousInfo->previousLocation = newLoc;

			auto newIcosaCoord = FIcosaPointCoord::convertPositionToIcosaPoint(newLoc, this->N_divisions);

			if (newIcosaCoord == previousInfo->previousIcosaCoord)
				return; // Moved, but inside of the same chunk

			if (player->GetRemoteRole() == ROLE_AutonomousProxy)
			{
				bool isServer = this->owner->HasAuthority();
				int asdf = 1;
			}

			DeMarkChunkAndNeighbours(previousInfo->previousIcosaCoord);

			previousInfo->previousIcosaCoord = newIcosaCoord;
			
			MarkChunkCoordAndNeighbours(newIcosaCoord);

			// Update new chunk

			// TODO 
			// 1) Mark all previous chunk with -1 
			// 2) Generate and mark all new chunks with +1

			//ModifyChunk(newIcosaCoord);
			
			return;
		}
		
		void ClearInactiveChunks()
		{
			// TODO optimize to changed only without the need to traverse the entire map!
			for (auto it = coordToTileMap.begin(); it != coordToTileMap.end(); )// no ++ because deletion also moves it
			{
				auto infoPointer = it->second;
				if (infoPointer->playersInRadius <= 0)
				{
					infoPointer->tileObject->Destroy(); // Destroy Actor 
					it = coordToTileMap.erase(it);  // Remove from map
					delete infoPointer;
				}
				else
				{
					++it;
				}
			}
		}

		void ClearInactivePlayers()
		{
			for (auto it = uniqueIDToPlayerInfoMap.begin(); it != uniqueIDToPlayerInfoMap.end(); ) // no ++ because deletion also moves it
			{
				auto infoPointer = it->second;
				if (infoPointer->onServer == false)
				{
					it = uniqueIDToPlayerInfoMap.erase(it);

					delete infoPointer;
				}
				else {
					++it;
				}
			}
		}
		
		void ToggleUpdateAllPlayers(TArray<ALogosCharacter*>& allPlayers)
		{
			EmulatePlayerDisconnect();  // minus one for all player, e.g., waiting for them to confirm that they are here

			for (auto pl : allPlayers)
			{
				ToggleUpdatePlayer(pl);  
			}

			ClearInactivePlayers(); // if did not received player character, delete it from tracking to optimize resources!

			// TODO optimize from global to changed only
			ClearInactiveChunks(); // destroy all chunks that did not get the neccessary number of players (<= 0)
			
		}
	};

	ChunkTrackingSystem chunkTrackingSystem;


	void UpdateChunks(std::set<FIcosaPointCoord>& toAdd);


	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coordinates")
	FIcosaPointCoord ConvertLocationToIcosaCoord(FVector playerLocation);


	//UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Coordinates")
	ASphereGridTile* CreateIcosaTile(FIcosaPointCoord icosaCoord);

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Coordinates")
	bool UpdateChunksTimerCall();


	
	


	double totalTime = 0.0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




};
