// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorManager.h"




// Sets default values
AFloorManager::AFloorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateMap();

	// Assign the team colour
	CollectPlayersAndBombs();
}

// Called every frame
void AFloorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageBombs();
	ManagePlayers();
	ManageCubes();
}

void AFloorManager::GenerateMap()
{
	for(int y = 0; y < MapSizeY; y++)
	{
		for(int x = 0; x < MapSizeX; x++)
		{
			// Spawn tiles
			FVector SpawnLocation = this->GetActorLocation()+FVector(x*TileScale*100,y*TileScale*100,0);
			FRotator SpawnRotation = FRotator(0,0,0);
			TilesArray.Add(GetWorld()->SpawnActor<AColourFloor>(TileClass, SpawnLocation, SpawnRotation));
		}
	}
}

void AFloorManager::CollectPlayersAndBombs()
{
	// Find all players in the game world
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);

	// Collect all players
	for(AActor* Actor: Actors)
	{
		if(Cast<AAssignment1BCharacter>(Actor))
		{
			PlayersArray.Add(Cast<AAssignment1BCharacter>(Actor));
		}
	}
	
	// Assign a team color to each player and spawn their owned bombs
	int PlayerIndex = 0;
	for(AAssignment1BCharacter* EachPlayer:PlayersArray)
	{
		if(PlayerIndex <= 1)
		{
			EachPlayer->SpawnBomb(FLinearColor::Red);
			BombsArray.Add(EachPlayer->SpawnedBomb);
		}
		else
		{
			EachPlayer->SpawnBomb(FLinearColor::Blue);
			BombsArray.Add(EachPlayer->SpawnedBomb);
		}
		PlayerIndex += 1;
	}
}

void AFloorManager::ManageBombs()
{
	for(int i = 0; i < BombsArray.Num(); i++)
	{
		if(BombsArray[i]->bIsActivated)
		{
			if(int TileIndexInArray; TilesArray.Find(BombsArray[i]->TilePlaced, TileIndexInArray))
			{
				BombsArray[i]->TilesImpacted = GenerateTilesImpacted(TileIndexInArray);
				
			}
		}
		// Start to detonate cubes with the same colour
		if(BombsArray[i]->bIsStartingToDetonateCubes)
		{
			CheckCubesImpactedByBombs(BombsArray[i]);
		}
	}
}

TArray<AColourFloor*> AFloorManager::GenerateTilesImpacted(int TilePlacedIndex)
{
	TArray<AColourFloor*> TilesImpacted;

	// Check whether the tile is at the edge
	TilesImpacted.Add(TilesArray[TilePlacedIndex]);
	
	if(TilePlacedIndex%MapSizeX!=0)
	{
		TilesImpacted.Add(TilesArray[TilePlacedIndex-1]);
	}
	if((TilePlacedIndex+1)%MapSizeX!=0)
	{
		TilesImpacted.Add(TilesArray[TilePlacedIndex+1]);
	}
	if(TilePlacedIndex>=MapSizeX)
	{
		TilesImpacted.Add(TilesArray[TilePlacedIndex-MapSizeX]);
	}
	if(TilePlacedIndex<(MapSizeY-1)*MapSizeX)
	{
		TilesImpacted.Add(TilesArray[TilePlacedIndex+MapSizeX]);
	}
	return TilesImpacted;
}

void AFloorManager::CheckCubesImpactedByBombs(AColourBomb* TargetBomb)
{
	for(const AColourFloor* EachTileImpacted:TargetBomb->TilesImpacted)
	{
		for(AColourCube* EachCube:CubesArray)
		{
			if(EachCube->TileSpawnedAt == EachTileImpacted&&EachCube->ColourDisplayed == TargetBomb->ColourDisplayed)
			{
				EachCube->bIsWaitingForDestroy = true;
			}
		}
	}
	TargetBomb->bIsStartingToDetonateCubes = false;
}

void AFloorManager::ManagePlayers()
{
	for(int i = 0; i < PlayersArray.Num(); i++)
	{
		if(PlayersArray[i]->bIsSpawningCube)
		{
			SpawnCubes(PlayersArray[i]->SpawnCubeColour, PlayersArray[i]->CubeSpawnLocation);
			PlayersArray[i]->bIsSpawningCube = false;
		}
	}
}

void AFloorManager::SpawnCubes(const FLinearColor SpawnCubeColour, const FVector CubeSpawnLocation)
{
	AColourCube* SpawnedCube = GetWorld()->SpawnActor<AColourCube>(CubeClass,CubeSpawnLocation,FRotator(0,0,0));
	SpawnedCube->InitializeColour(SpawnCubeColour);
	CubesArray.Add(SpawnedCube);
}

void AFloorManager::ManageCubes()
{
	TArray<AColourCube*> CubesToDestroy;
	for(AColourCube* EachCube:CubesArray)
	{
		if(EachCube->bIsWaitingForDestroy)
		{
			CubesToDestroy.Add(EachCube);
		}
	}

	for(int i = 0; i < CubesToDestroy.Num(); i++)
	{
		CubesArray.Remove(CubesToDestroy[i]);
		if(CubesToDestroy[i]->TileSpawnedAt)
		{
			if(int TileIndexInArray; TilesArray.Find(CubesToDestroy[i]->TileSpawnedAt, TileIndexInArray))
			{
				CubesToDestroy[i]->TilesImpacted = GenerateTilesImpacted(TileIndexInArray);
			}
		}
	}
}








