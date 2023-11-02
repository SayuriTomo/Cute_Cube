// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemRefreshable.h"


// Sets default values
AItemRefreshable::AItemRefreshable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Mesh"));
	SetRootComponent(MainMesh);

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBoxComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AItemRefreshable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemRefreshable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator NextRotation = this->GetActorRotation() + FRotator(0,5,0);
	SetActorRotation(NextRotation);
	
	if(bIsActive)
	{
		ProcessPlayerCollision();
	}
	else
	{
		MainMesh->SetVisibility(false);
		if(TimeRemainedToRefresh<=0)
		{
			bIsActive = true;
			MainMesh->SetVisibility(true);
		}
		TimeRemainedToRefresh -= DeltaTime;
	}
}

void AItemRefreshable::ProcessPlayerCollision()
{
}

