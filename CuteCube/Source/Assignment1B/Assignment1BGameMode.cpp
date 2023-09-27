// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment1BGameMode.h"
#include "Assignment1BCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAssignment1BGameMode::AAssignment1BGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAssignment1BGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AAssignment1BGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if(AAssignment1BCharacter* Character = Cast<AAssignment1BCharacter>(NewPlayer->GetPawn()))
	{
		Character->StartController();
	}
}
