// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Assignment1B/ColourCube/ColourCube.h"
#include "Assignment1B/ColourFloor/ColourFloor.h"
#include "Assignment1B/ColourInterface/ColourInterface.h"
#include "EnhancedBomb.generated.h"

UCLASS()
class ASSIGNMENT1B_API AEnhancedBomb : public AActor, public IColourInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnhancedBomb();

	UPROPERTY(EditAnywhere)UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* TriggerBoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float TimeRemainedToDestroy;
	UPROPERTY(EditAnywhere)float TimeDuration = 5.0f;

	void ProcessToDyeSurrounding();

};
