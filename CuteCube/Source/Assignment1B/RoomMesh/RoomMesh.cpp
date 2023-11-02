// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomMesh.h"

// Sets default values
ARoomMesh::ARoomMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Room Mesh"));
	SetRootComponent(RoomMesh);
}

// Called when the game starts or when spawned
void ARoomMesh::BeginPlay()
{
	Super::BeginPlay();

	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		RoomMesh->SetMaterial(0,MaterialInstance);
	}
	
}

// Called every frame
void ARoomMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomMesh::ChangeColour(FLinearColor ColourToChange)
{
	if(MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourToChange);
	}
}

