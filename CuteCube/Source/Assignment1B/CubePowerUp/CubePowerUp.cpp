// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePowerUp.h"

void ACubePowerUp::ProcessPlayerCollision()
{
	Super::ProcessPlayerCollision();
	
	TArray<AActor*>Actors;
	const TSubclassOf<AAssignment1BCharacter> PlayerClass;
	GetOverlappingActors(Actors,PlayerClass);
	for (AActor* Actor:Actors)
	{
		if(Cast<AAssignment1BCharacter>(Actor))
		{
			Cast<AAssignment1BCharacter>(Actor)->CubeAbilityUnlock();
			bIsActive = false;
			TimeRemainedToRefresh = TimeRequiredToRefresh;
		}
	}
}
