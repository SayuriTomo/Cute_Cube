// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment1B/Assignment1BCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT1B_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()TArray<UTextBlock*>MatchTimeArray; // Store each match time block
	UPROPERTY()TArray<UTextBlock*>PrepareTimeArray; // Store each prepare time block
	UPROPERTY()TArray<UImage*>CrossHairArray; // Store each cross hair block

	void InitializeArray();
	
	// Player 1 Screen
	UPROPERTY(meta = (BindWidget))class UTextBlock* MatchTime1;
	UPROPERTY(meta = (BindWidget))class UTextBlock* PrepareTime1;
	UPROPERTY(meta = (BindWidget))class UImage* CrossHair1;
	
	// Player 2 Screen
	UPROPERTY(meta = (BindWidget))class UTextBlock* MatchTime2;
	UPROPERTY(meta = (BindWidget))class UTextBlock* PrepareTime2;
	UPROPERTY(meta = (BindWidget))class UImage* CrossHair2;
	
	// Player 3 Screen
	UPROPERTY(meta = (BindWidget))class UTextBlock* MatchTime3;
	UPROPERTY(meta = (BindWidget))class UTextBlock* PrepareTime3;
	UPROPERTY(meta = (BindWidget))class UImage* CrossHair3;
	
	// Player 4 Screen
	UPROPERTY(meta = (BindWidget))class UTextBlock* MatchTime4;
	UPROPERTY(meta = (BindWidget))class UTextBlock* PrepareTime4;
	UPROPERTY(meta = (BindWidget))class UImage* CrossHair4;

	// Manage match time module
	void ManagerMatchTime();
	void HideMatchTime();
	void ShowMatchTime();

	// Manage prepare time and cross hair module
	void ManagePrepareTimeAndCrossHair();
	void HidePrepareTime();
	void ShowPrepareTime();

	// Manage winner module
	bool StartTimer = false;
	void ManagerWinner();
	void HideWinner();
	
	FTimerHandle BasicTimerHandle;
	float CollectPlayersTimeRequired = 0.1f; // Delay to collect the data
	float DisplayWinnerTimeRequired = 5.0f; // Time to display the winner

	// Collect players module 
	void CollectPlayers();
	UPROPERTY()TArray<AAssignment1BCharacter*> PlayersArray; // Store each player
	bool bIsFinishedCollecting = false;

	
};
