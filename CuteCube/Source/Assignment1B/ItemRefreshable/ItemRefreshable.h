// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Assignment1B/Assignment1BCharacter.h"
#include "ItemRefreshable.generated.h"

UCLASS()
class ASSIGNMENT1B_API AItemRefreshable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemRefreshable();
	
	UPROPERTY(EditAnywhere)UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* TriggerBoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool bIsActive = true;
	
	float TimeRemainedToRefresh;
	UPROPERTY(EditAnywhere)float TimeRequiredToRefresh = 25.0f;
	virtual void ProcessPlayerCollision();
};
