// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/ColourFloor/ColourFloor.h"
#include "GameFramework/Actor.h"
#include "Assignment1B/ColourInterface/ColourInterface.h"
#include "ColourBomb.generated.h"

UCLASS()
class ASSIGNMENT1B_API AColourBomb : public AActor,public IColourInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColourBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Record which tile that the bomb is placed
	UPROPERTY()AColourFloor* TilePlaced;
	
	// Be activated module
	bool bIsActivated = false; // Check whether this bomb is activated
	void BeActivated(AColourFloor* Tile);
	UPROPERTY()TArray<AColourFloor*> TilesImpacted; // Record the tiles impacted by this activation

	FTimerHandle BeActivatedTimerHandle; // Control the activation time required
	UPROPERTY(EditAnywhere)float ActivationTimeRequired = 3.0f; // Default time required is 3 seconds
	void DiffuseColour();
	bool bIsStartingToDetonateCubes = false;
};
