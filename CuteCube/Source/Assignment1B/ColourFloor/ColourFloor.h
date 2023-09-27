// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/ColourInterface/ColourInterface.h"
#include "GameFramework/Actor.h"
#include "ColourFloor.generated.h"

UCLASS()
class ASSIGNMENT1B_API AColourFloor : public AActor,public IColourInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColourFloor();
	
	UPROPERTY(EditAnywhere)UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	
	UPROPERTY()UMaterialInstanceDynamic* MaterialInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ChangeColour(FLinearColor ColourToChange);

	// Check whether there is a bomb on this tile
	bool bIsBombPlacedOn = false;

	// Check whether there is a cube on this tile
	bool bIsCubePlacedOn = false;

	FLinearColor ColourOfCubePlaced;
};
