// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedBomb.h"


// Sets default values
AEnhancedBomb::AEnhancedBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Mesh"));
	SetRootComponent(MainMesh);

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnhancedBomb::BeginPlay()
{
	Super::BeginPlay();
	TimeRemainedToDestroy = TimeDuration;
}

// Called every frame
void AEnhancedBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator NextRotation = this->GetActorRotation() + FRotator(0,5,0);
	SetActorRotation(NextRotation);
	
	if(TimeRemainedToDestroy<=0)
	{
		this->Destroy();
	}
	TimeRemainedToDestroy -= DeltaTime;

	ProcessToDyeSurrounding();
}

void AEnhancedBomb::ProcessToDyeSurrounding()
{
	TArray<AActor*>Actors;
	const TSubclassOf<AActor> OverlappingClass;
	GetOverlappingActors(Actors,OverlappingClass);
	for (AActor* Actor:Actors)
	{
		if(Cast<AColourFloor>(Actor))
		{
			Cast<AColourFloor>(Actor)->ChangeBaseColour(ColourDisplayed);
		}
		if(Cast<AColourCube>(Actor))
		{
			Cast<AColourCube>(Actor)->bIsWaitingForDestroy = true;
		}
	}
}

