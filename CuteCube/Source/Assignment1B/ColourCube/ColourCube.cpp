// Fill out your copyright notice in the Description page of Project Settings.


#include "ColourCube.h"

// Sets default values
AColourCube::AColourCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	SetRootComponent(CubeMesh);
}

// Called when the game starts or when spawned
void AColourCube::BeginPlay()
{
	Super::BeginPlay();

	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		CubeMesh->SetMaterial(0,MaterialInstance);
	}
	GetWorld()->GetTimerManager().SetTimer(ReadyTimerHandle, this, &AColourCube::AllSet, ReadyTimeRequired, true);
	
}

// Called every frame
void AColourCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsReady&&GetVelocity()==FVector(0,0,0))
	{
		UpdateSurroundings();
		CheckSurroundings();
	}
	
	
	if(bIsWaitingForDestroy)
	{
		if(!bIsStartingToDestroy)
		{
			/*
			if(DestroyMaterialClass)
			{
				MaterialInstance = UMaterialInstanceDynamic::Create(DestroyMaterialClass, this);
				CubeMesh->SetMaterial(0,MaterialInstance);
				MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourDisplayed);
			}
			*/

			bIsStartingToDestroy = true;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AColourCube::DestroyItself, DestroyTimeRequired, true);
		}
	}
}

void AColourCube::InitializeColour(FLinearColor ColourToInitialize)
{
	ColourDisplayed = ColourToInitialize;
	if(MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourDisplayed);
	}
}

void AColourCube::UpdateSurroundings()
{
	for(int i = 0; i<6; i++)
	{
		CallMyTrace(i);
	}
}

bool AColourCube::Trace(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End,
                        FHitResult& HitOut, ECollisionChannel CollisionChannel, bool ReturnPhysMat)
{
	// The World parameter refers to our game world (map/level) If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel (
		HitOut, //result
		Start, //start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

void AColourCube::CallMyTrace(int DirectionNumber)
{
	// Initialize the vector
	const FVector Start = this->GetActorLocation();
	FVector End;

	// Totally 6 directions
	switch (DirectionNumber)
	{
		case 0:
			{
				End = Start + FVector(190,0,0);
				break;
			}
		case 1:
			{
				End = Start - FVector(190,0,0);
				break;
			}
		case 2:
			{
				End = Start + FVector(0,190,0);
				break;
			}
		case 3:
			{
				End = Start - FVector(0,190,0);
				break;
			}
		case 4:
			{
				End = Start + FVector(0,0,190);
				break;
			}
		case 5:
			{
				End = Start - FVector(0,0,190);
				break;
			}
		default:{}
	}
	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);
	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;
	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor())
		{
			ProcessUpdateSurroundingsHit(HitData);
		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
	}
}

void AColourCube::ProcessUpdateSurroundingsHit(FHitResult& HitOut)
{
	if(AColourCube* HitCube = Cast<AColourCube>(HitOut.GetActor()))
	{
		if(HitCube->ColourDisplayed == this->ColourDisplayed&&HitCube->bIsReady)
		{
			if(!SameColourCubesArray.Contains(HitCube))
			{
				SameColourCubesArray.Add(HitCube);
			}
		}
	}
	if(AColourFloor* HitFloorTile = Cast<AColourFloor>(HitOut.GetActor()))
	{
		HitFloorTile->bIsCubePlacedOn = true;
		HitFloorTile->ColourOfCubePlaced = ColourDisplayed;
		TileSpawnedAt = HitFloorTile;
	}
}

void AColourCube::CheckSurroundings()
{
	for(AColourCube* EachCube:SameColourCubesArray)
	{
		if(EachCube->bIsWaitingForDestroy)
		{
			this->bIsWaitingForDestroy = true;
		}
	}
	if(SameColourCubesArray.Num()>=2)
	{
		bIsWaitingForDestroy = true;
	}

	SameColourCubesArray.Empty();
}

void AColourCube::DestroyItself()
{
	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = this->GetActorRotation();
	UNiagaraComponent* DestroyCubeComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_DestroyCube, SpawnLocation, SpawnRotation);
	DestroyCubeComponent->SetNiagaraVariableLinearColor(FString("ColourDisplayed"),ColourDisplayed);
	SetActorHiddenInGame(true);
	CubeMesh->SetCastShadow(false);
	DiffuseColour();
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
	
	this->Destroy();
}

void AColourCube::AllSet()
{
	bIsReady = true;

	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(ReadyTimerHandle);
}

void AColourCube::DiffuseColour()
{
	for(AColourFloor* EachTile : TilesImpacted)
	{
		EachTile->ChangeColour(ColourDisplayed);
	}
	TileSpawnedAt->bIsCubePlacedOn = false;
}


