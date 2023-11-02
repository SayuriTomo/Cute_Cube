// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/ItemRefreshable/ItemRefreshable.h"
#include "BombPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT1B_API ABombPowerUp : public AItemRefreshable
{
	GENERATED_BODY()
	
public:
	virtual void ProcessPlayerCollision() override;
	
};
