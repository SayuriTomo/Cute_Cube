// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/ItemRefreshable/ItemRefreshable.h"
#include "CubePowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT1B_API ACubePowerUp : public AItemRefreshable
{
	GENERATED_BODY()
	
public:
	virtual void ProcessPlayerCollision() override;
};
