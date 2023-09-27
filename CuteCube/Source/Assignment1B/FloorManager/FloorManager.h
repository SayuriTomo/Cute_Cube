// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/Assignment1BCharacter.h"
#include "Assignment1B/ColourBomb/ColourBomb.h"
#include "GameFramework/Actor.h"
#include "Assignment1B/ColourFloor/ColourFloor.h"
#include "Kismet/GameplayStatics.h"
#include "FloorManager.generated.h"

UCLASS()
class ASSIGNMENT1B_API AFloorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Store the number of floor tiles in x-axis
	UPROPERTY(EditAnywhere)int MapSizeX;
	
	// Store the number of floor tiles in y-axis
	UPROPERTY(EditAnywhere)int MapSizeY;

	// Store the scale of tile to ensure each spawn location
	UPROPERTY(EditAnywhere)float TileScale;

private:
	// Spawn tile module
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourFloor> TileClass;
	void GenerateMap();
	UPROPERTY()TArray<AColourFloor*> TilesArray; // Store each tile
	
	// Collect player and bomb module
	void CollectPlayersAndBombs();
	UPROPERTY()TArray<AColourBomb*> BombsArray; // Store each bomb
	UPROPERTY()TArray<AAssignment1BCharacter*> PlayersArray; // Store each player
	
	// Manage bomb module
	void ManageBombs();
	TArray<AColourFloor*> GenerateTilesImpacted(int TilePlacedIndex);
	void CheckCubesImpactedByBombs(AColourBomb* TargetBomb);
	FTimerHandle DetonateCubesTimerHandle;
	UPROPERTY(EditAnywhere)float DetonateCubesRequired = 3.0f;

	
	// Manage player module to spawn the colour cube
	void ManagePlayers();
	void SpawnCubes(const FLinearColor SpawnCubeColour, const FVector CubeSpawnLocation);
	UPROPERTY()TArray<AColourCube*> CubesArray;
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourCube> CubeClass;

	void ManageCubes();
};
