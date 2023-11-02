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
	if(bIsMainFloorManager)
	{
		for(AFloorManager* SubFloor:SubFloorManagers)
		{
			SubFloor->GenerateMap();
		}
	}
}

// Called every frame
void AFloorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsActivated)
	{
		ManagePlayers();
		ManageBombs();
		ManageCubes();
		if(bIsMainFloorManager)
		{
			ProcessPrepareStage(DeltaTime);
			ProcessMatchStage(DeltaTime);
		}
	}
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

void AFloorManager::ReceivePlayersAndBombs(TArray<AColourBomb*> Bombs, TArray<AAssignment1BCharacter*> Players)
{
	// Receive the data from map manager
	BombsArray = Bombs;
	PlayersArray = Players;
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
		if(BombsArray[i]->bIsStartingToDetonateCubes&&TilesArray.Contains(BombsArray[i]->TilePlaced))
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
			if(EachCube->TileSpawnedAt == EachTileImpacted)
			{
				if(EachCube->ColourDisplayed == TargetBomb->ColourDisplayed)
				{
					EachCube->bIsWaitingForDestroy = true;
				}
				if(EachCube->ColourDisplayed == FLinearColor::Gray)
				{
					EachCube->ChangeColour(TargetBomb->ColourDisplayed);
				}
			}
			
		}
	}
	TargetBomb->bIsStartingToDetonateCubes = false;
}

void AFloorManager::ManagePlayers()
{
	for(int i = 0; i < PlayersArray.Num(); i++)
	{
		// In prepare stage
		if(bIsPreparingToStart)
		{
			PlayersArray[i]->PrepareTimeRemain = CurrentPrepareTime;
			PlayersArray[i]->bIsInPrepare = true;
			PlayersArray[i]->SetActorLocation(PlayersArray[i]->RespawnLocation);
		}

		// In Match
		if(bIsMatchStart)
		{
			PlayersArray[i]->bIsInMatch = true;
			PlayersArray[i]->MatchTimeRemain = CurrentMatchTime;
			PlayersArray[i]->bIsInPrepare = false;
		}

		// When the player choose to spawn a cube
		if(PlayersArray[i]->bIsSpawningCube)
		{
			if(TilesArray.Contains(PlayersArray[i]->CubeSpawnTile))
			{
				SpawnCubes(PlayersArray[i]->SpawnCubeColour, PlayersArray[i]->CubeSpawnLocation);
				PlayersArray[i]->bIsSpawningCube = false;
			}
		}

		// When the player is respawning
		if(PlayersArray[i]->bIsRespawning)
		{
			PlayersArray[i]->bIsInPrepare = true;
			PlayersArray[i]->SetActorLocation(PlayersArray[i]->RespawnLocation);
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
	// Check if the cubes will be destroyed
	TArray<AColourCube*> CubesToDestroy;
	for(AColourCube* EachCube:CubesArray)
	{
		if(EachCube->bIsWaitingForDestroy)
		{
			CubesToDestroy.Add(EachCube);
		}
	}

	// Calculate the tiles impacted by the cubes destroyed
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

void AFloorManager::EndMatch()
{
	bIsActivated = false;
	
	PlayersArray[0]->WhichTeamWin = EvaluateResult();
	
	ClearData();
}

int AFloorManager::EvaluateResult()
{
	int RedTilesNum = 0, BlueTilesNum = 0;
	
	for(int i = 0; i < TilesArray.Num(); i++)
	{
		if(TilesArray[i]->ColourDisplayed == FLinearColor::Red)
		{
			RedTilesNum += 1;
		}
		else if(TilesArray[i]->ColourDisplayed == FLinearColor::Blue)
		{
			BlueTilesNum += 1;
		}
	}
	for(AFloorManager* SubFloor:SubFloorManagers)
	{
		for(int i = 0; i < SubFloor->TilesArray.Num(); i++)
		{
			if(SubFloor->TilesArray[i]->ColourDisplayed == FLinearColor::Red)
			{
				RedTilesNum += 1;
			}
			else if(SubFloor->TilesArray[i]->ColourDisplayed == FLinearColor::Blue)
			{
				BlueTilesNum += 1;
			}
		}
	}
	
	if(RedTilesNum>BlueTilesNum)
	{
		return 1;
	}
	else if(RedTilesNum<BlueTilesNum)
	{
		return 2;
	}
	return 0;
}

void AFloorManager::ClearData()
{
	for(int i = 0; i < PlayersArray.Num(); i++)
	{
		PlayersArray[i]->bIsInMatchEnd = true;
		PlayersArray[i]->bIsInMatch = false;
		PlayersArray[i]->bIsSpawnSameCubesUnlocked = false;
		PlayersArray[i]->bIsSpawnEnhancedBombUnlocked = false;
	}

	if(RoomMesh)RoomMesh->ChangeColour(FLinearColor::Gray);
	
	ClearSelfData();
	
	for(AFloorManager* SubFloor:SubFloorManagers)
	{
		SubFloor->ClearSelfData();
	}
}

void AFloorManager::ClearSelfData()
{
	for(int i = 0; i < TilesArray.Num(); i++)
	{
		TilesArray[i]->ChangeBaseColour(FLinearColor::Gray);
		TilesArray[i]->ChangeEdgeColour(FLinearColor::White);
	}
	
	for(int i = 0; i < CubesArray.Num(); i++)
	{
		CubesArray[i]->Destroy();
	}

	CubesArray.Empty();
	BombsArray.Empty();
	PlayersArray.Empty();
}

void AFloorManager::ProcessPrepareStage(float DeltaTime)
{
	if(bIsPreparingToStart)
	{
		CurrentPrepareTime -= DeltaTime;
		if(CurrentPrepareTime <= 0)
		{
			bIsPreparingToStart = false;
			bIsMatchStart = true;
			CurrentMatchTime = MatchTimeRequired;
		}
	}
}

void AFloorManager::ProcessMatchStage(float DeltaTime)
{
	if(bIsMatchStart)
	{
		CurrentMatchTime -= DeltaTime;
		FrontTeam = EvaluateResult();
		ManageRoom();
		if(CurrentMatchTime <= 0)
		{
			bIsMatchStart = false;
			EndMatch();
		}
	}
}

void AFloorManager::ManageRoom()
{
	if(RoomMesh)
	{
		switch (FrontTeam)
		{case 0:
			{
				RoomMesh->ChangeColour(FLinearColor::Gray);
				break;
			}
		case 1:
			{
				RoomMesh->ChangeColour(FLinearColor::Red);
				break;
			}
		case 2:
			{
				RoomMesh->ChangeColour(FLinearColor::Blue);
				break;
			}
		default:{}
		}
	}
}








