// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ColourInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UColourInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSIGNMENT1B_API IColourInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Represent the colour of this actor
	FLinearColor ColourDisplayed;
};
