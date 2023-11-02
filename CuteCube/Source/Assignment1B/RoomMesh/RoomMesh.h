// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomMesh.generated.h"

UCLASS()
class ASSIGNMENT1B_API ARoomMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomMesh();

	UPROPERTY(EditAnywhere)UStaticMeshComponent* RoomMesh;
	
	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	
	UPROPERTY()UMaterialInstanceDynamic* MaterialInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeColour(FLinearColor ColourToChange);
};

