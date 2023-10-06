// Fill out your copyright notice in the Description page of Project Settings.


#include "ColourBomb.h"

// Sets default values
AColourBomb::AColourBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColourBomb::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AColourBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AColourBomb::BeActivated(AColourFloor* Tile)
{
	// The initial setting when the colour bomb is activated
	TilePlaced = Tile;
	TilePlaced->bIsBombPlacedOn = true;
	bIsActivated = true;
	GetWorld()->GetTimerManager().SetTimer(BeActivatedTimerHandle, this, &AColourBomb::DiffuseColour, ActivationTimeRequired, true);
}

void AColourBomb::DiffuseColour()
{
	// Change the colour of each tile impacted
	for(AColourFloor* EachTile : TilesImpacted)
	{
		EachTile->ChangeBaseColour(ColourDisplayed);
	}

	bIsStartingToDetonateCubes = true;
	
	// Re-initialize the bomb and the tile placed
	bIsActivated = false;
	
	TilePlaced->bIsBombPlacedOn = false;

	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(BeActivatedTimerHandle);
}

