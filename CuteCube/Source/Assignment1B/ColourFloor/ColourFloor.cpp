// Fill out your copyright notice in the Description page of Project Settings.


#include "ColourFloor.h"

// Sets default values
AColourFloor::AColourFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	SetRootComponent(FloorMesh);

}

// Called when the game starts or when spawned
void AColourFloor::BeginPlay()
{
	Super::BeginPlay();
	// Create Material Instance
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		FloorMesh->SetMaterial(0,MaterialInstance);
	}
	// Initialize the original colour
	ChangeColour(FLinearColor::Gray);
}

// Called every frame
void AColourFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AColourFloor::ChangeColour(FLinearColor ColourToChange)
{
	if(!bIsCubePlacedOn||(bIsCubePlacedOn&&ColourOfCubePlaced==ColourToChange))
	{
		ColourDisplayed = ColourToChange;
		if(MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourDisplayed);
		}
	}
}

