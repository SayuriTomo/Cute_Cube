// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Assignment1B/ColourFloor/ColourFloor.h"
#include "GameFramework/Actor.h"
#include "Assignment1B/ColourInterface/ColourInterface.h"
#include "ColourCube.generated.h"

UCLASS()
class ASSIGNMENT1B_API AColourCube : public AActor,public IColourInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColourCube();

	UPROPERTY(EditAnywhere)UStaticMeshComponent* CubeMesh;
	
	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;

	//UPROPERTY(EditAnywhere)UMaterialInterface* DestroyMaterialClass;
	
	UPROPERTY()UMaterialInstanceDynamic* MaterialInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeColour(FLinearColor ColourToInitialize);

	UPROPERTY()AColourFloor* TileSpawnedAt;
	bool bIsWaitingForDestroy = false;
	UPROPERTY()TArray<AColourFloor*> TilesImpacted;
	
private:
	// Update surroundings module
	void UpdateSurroundings();
	bool Trace(UWorld* World,TArray<AActor*>& ActorsToIgnore,
				const FVector& Start,const FVector& End,
				FHitResult& HitOut,ECollisionChannel CollisionChannel,bool ReturnPhysMat);
	void CallMyTrace(int ActionNumber);
	void ProcessUpdateSurroundingsHit(FHitResult& HitOut);

	// Check surroundings module
	void CheckSurroundings();
	UPROPERTY()TArray<AColourCube*> SameColourCubesArray;

	// Destroy itself module
	void DestroyItself();
	bool bIsStartingToDestroy = false;
	UPROPERTY(EditAnywhere)UNiagaraSystem* NS_DestroyCube;
	FTimerHandle DestroyTimerHandle;
	UPROPERTY(EditAnywhere)float DestroyTimeRequired = 0.5f; // Default time required is 0.5 second

	// Ready module to ensure the same colour cubes can update in the same time
	void AllSet();
	bool bIsReady = false;
	FTimerHandle ReadyTimerHandle;
	UPROPERTY(EditAnywhere)float ReadyTimeRequired = 0.01f;

	void DiffuseColour();
};
