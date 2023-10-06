// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/FloorManager/FloorManager.h"
#include "Assignment1B/Assignment1BCharacter.h"
#include "Assignment1B/MapManager/MapManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapSelector.generated.h"

UCLASS()
class ASSIGNMENT1B_API AMapSelector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapSelector();

	UPROPERTY(EditAnywhere)UStaticMeshComponent* SelectorMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* TriggerBoxComponent;
	UPROPERTY(EditAnywhere)AFloorManager* FloorManagerOwned;
	UPROPERTY(EditAnywhere)AMapManager* ParentMapManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere) int PlayerNumRequired; // Check if all players choose the same map

	int UpdatePlayerNum();
	void StartMatch();
	void UpdatePlayerSpawnLocation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
