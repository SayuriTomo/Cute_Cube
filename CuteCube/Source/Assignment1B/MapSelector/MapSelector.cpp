// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelector.h"

// Sets default values
AMapSelector::AMapSelector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selector Mesh"));
	SetRootComponent(SelectorMesh);

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMapSelector::BeginPlay()
{
	Super::BeginPlay();
	
}

int AMapSelector::UpdatePlayerNum()
{
	int CurrentPlayerNum = 0;
	TArray<AActor*>Actors;
	const TSubclassOf<AAssignment1BCharacter> PlayerClass;
	GetOverlappingActors(Actors,PlayerClass);
	for (AActor* Actor:Actors)
	{
		if(Cast<AAssignment1BCharacter>(Actor))
		{
			CurrentPlayerNum += 1;
		}
	}
	return CurrentPlayerNum;
}

void AMapSelector::StartMatch()
{
	if(!FloorManagerOwned->bIsActivated)
	{
		FloorManagerOwned->GenerateMap();
		FloorManagerOwned->bIsActivated = true;
		ParentMapManager->WaitFloor->bIsActivated = false;
	}
	FloorManagerOwned->CurrentPrepareTime = FloorManagerOwned->PrepareTimeRequired;
	FloorManagerOwned->bIsPreparingToStart = true;
	FloorManagerOwned->ReceivePlayersAndBombs(ParentMapManager->BombsArray,ParentMapManager->PlayersArray);
	
	UpdatePlayerSpawnLocation();
}

void AMapSelector::UpdatePlayerSpawnLocation()
{
	int RedPlayerIndex = 0, BluePlayerIndex = 0;
	
	for(AAssignment1BCharacter* EachPlayer:ParentMapManager->PlayersArray)
	{
		if(EachPlayer->ColourDisplayed == FLinearColor::Red)
		{
			EachPlayer->RespawnLocation = FloorManagerOwned->
											SpawnLocationsArray[RedPlayerIndex]->
												GetActorLocation()+FVector(0,0,100);
			RedPlayerIndex += 1;
		}
		else if(EachPlayer->ColourDisplayed == FLinearColor::Blue)
		{
			EachPlayer->RespawnLocation = FloorManagerOwned->
											SpawnLocationsArray[BluePlayerIndex+2]->
												GetActorLocation()+FVector(0,0,100);
			BluePlayerIndex += 1;
		}
	}
}

// Called every frame
void AMapSelector::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	if(UpdatePlayerNum() == PlayerNumRequired && ParentMapManager)
	{
		StartMatch();
	}
}

