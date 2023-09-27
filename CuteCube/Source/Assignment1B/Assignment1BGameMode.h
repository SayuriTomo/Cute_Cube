// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ColourInterface/ColourInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Assignment1BGameMode.generated.h"

UCLASS(minimalapi)
class AAssignment1BGameMode : public AGameModeBase, public IColourInterface
{
	GENERATED_BODY()

public:
	AAssignment1BGameMode();
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};



