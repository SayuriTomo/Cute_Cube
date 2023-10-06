// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/FloorManager/FloorManager.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

UCLASS()
class ASSIGNMENT1B_API AMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapManager();

	UPROPERTY(EditAnywhere)AFloorManager* WaitFloor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeFloor();
	
	// Generate players and bombs module
	FTimerHandle GeneratePlayersTimerHandle;
	float GeneratePlayersTimeRequired = 0.1f; // Delay to collect the data
	void GeneratePlayersAndBombs();
	UPROPERTY()TArray<AColourBomb*> BombsArray; // Store each bomb
	UPROPERTY()TArray<AAssignment1BCharacter*> PlayersArray; // Store each player

	// Players return to wait floor
	void CheckIfPlayersEndMatch();
};
