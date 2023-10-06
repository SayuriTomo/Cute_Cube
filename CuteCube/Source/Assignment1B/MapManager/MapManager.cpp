// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"

// Sets default values
AMapManager::AMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeFloor();
}

// Called every frame
void AMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!PlayersArray.IsEmpty())
	{
		CheckIfPlayersEndMatch();
	}
	
}

void AMapManager::InitializeFloor()
{
	// Generate the wait room floor
	WaitFloor->GenerateMap();
	WaitFloor->bIsActivated = true;
	GetWorld()->GetTimerManager().SetTimer(GeneratePlayersTimerHandle, this, &AMapManager::GeneratePlayersAndBombs, GeneratePlayersTimeRequired, true);
}

void AMapManager::GeneratePlayersAndBombs()
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
	
	for(int i = 0; i < PlayersArray.Num(); i++)
	{
		PlayersArray[i]->RespawnLocation = WaitFloor->SpawnLocationsArray[i]->GetActorLocation()+FVector(0,0,100);
		PlayersArray[i]->SetActorLocation(PlayersArray[i]->RespawnLocation);
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

	// Transfer the data to floor manager
	WaitFloor->ReceivePlayersAndBombs(BombsArray,PlayersArray);
	
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(GeneratePlayersTimerHandle);
}

void AMapManager::CheckIfPlayersEndMatch()
{
	if(PlayersArray[0]->bIsInMatchEnd)
	{
		for(int i = 0; i < PlayersArray.Num(); i++)
		{
			PlayersArray[i]->RespawnLocation = WaitFloor->SpawnLocationsArray[i]->GetActorLocation()+FVector(0,0,100);
			PlayersArray[i]->SetActorLocation(PlayersArray[i]->RespawnLocation);
		}
	}
	WaitFloor->bIsActivated = true;
}

