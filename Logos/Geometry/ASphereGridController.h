// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASphereGridController.generated.h"

UCLASS()
class LOGOS_API AASphereGridController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASphereGridController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
