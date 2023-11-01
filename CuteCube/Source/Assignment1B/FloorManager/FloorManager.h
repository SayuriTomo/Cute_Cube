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

	// Check if the floor manager has already generate the tiles
	bool bIsActivated = false;
	
	UPROPERTY(EditAnywhere)int MapSizeX; // Store the number of floor tiles in x-axis

	UPROPERTY(EditAnywhere)int MapSizeY; // Store the number of floor tiles in y-axis

	UPROPERTY(EditAnywhere)float TileScale; // Store the scale of tile to ensure each spawn location

	UPROPERTY(EditAnywhere)TArray<AActor*> SpawnLocationsArray; // Store the location that each player will spawn

	UPROPERTY(EditAnywhere)float MatchTimeRequired = 120.0f; // Store the time of the match required
	float CurrentMatchTime;

	UPROPERTY(EditAnywhere)float PrepareTimeRequired = 5.0f; // Store the time of the prepare stage required
	float CurrentPrepareTime;
	
	bool bIsMatchStart = false;
	bool bIsPreparingToStart = false;
	
	// Spawn tile module
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourFloor> TileClass;
	void GenerateMap();
	
	// Receive players and bombs from map manager
	void ReceivePlayersAndBombs(TArray<AColourBomb*> Bombs,TArray<AAssignment1BCharacter*> Players);

	UPROPERTY(EditAnywhere)bool bIsMainFloorManager;
	UPROPERTY(EditAnywhere)TArray<AFloorManager*> SubFloorManagers;

private:
	UPROPERTY()TArray<AColourFloor*> TilesArray; // Store each tile
	UPROPERTY()TArray<AColourBomb*> BombsArray; // Store each bomb
	UPROPERTY()TArray<AAssignment1BCharacter*> PlayersArray; // Store each player
	
	// Manage bombs module
	void ManageBombs();
	TArray<AColourFloor*> GenerateTilesImpacted(int TilePlacedIndex);
	void CheckCubesImpactedByBombs(AColourBomb* TargetBomb);
	FTimerHandle DetonateCubesTimerHandle;
	UPROPERTY(EditAnywhere)float DetonateCubesRequired = 3.0f;
	
	// Manage players module to spawn the colour cube
	void ManagePlayers();
	void SpawnCubes(const FLinearColor SpawnCubeColour, const FVector CubeSpawnLocation);
	UPROPERTY()TArray<AColourCube*> CubesArray; // Store each cube
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourCube> CubeClass;

	// Manage cubes module
	void ManageCubes();

	// End the match and evaluate the result
	void EndMatch();
	int EvaluateResult();
	
	// Clear the arrays when the manager is unactivated
	void ClearData();
	void ClearSelfData();

	void ProcessPrepareStage(float DeltaTime);

	void ProcessMatchStage(float DeltaTime);
	
	// Manage tiles module
	int FrontTeam = 0; // 0 means draw, 1 means red front, 2 means blue front
	void ManageTiles();
};
