// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"



void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeArray();
	HideMatchTime();
	HidePrepareTime();
	GetWorld()->GetTimerManager().SetTimer(BasicTimerHandle, this, &UPlayerWidget::CollectPlayers, CollectPlayersTimeRequired, true);
}

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(bIsFinishedCollecting)
	{
		ManagerMatchTime();
		ManagePrepareTimeAndCrossHair();
		ManagerWinner();
	}
}

void UPlayerWidget::InitializeArray()
{
	MatchTimeArray.Add(MatchTime1);
	MatchTimeArray.Add(MatchTime2);
	MatchTimeArray.Add(MatchTime3);
	MatchTimeArray.Add(MatchTime4);

	PrepareTimeArray.Add(PrepareTime1);
	PrepareTimeArray.Add(PrepareTime2);
	PrepareTimeArray.Add(PrepareTime3);
	PrepareTimeArray.Add(PrepareTime4);

	CrossHairArray.Add(CrossHair1);
	CrossHairArray.Add(CrossHair2);
	CrossHairArray.Add(CrossHair3);
	CrossHairArray.Add(CrossHair4);
}


void UPlayerWidget::HideMatchTime()
{
	for(UTextBlock* EachBlock:MatchTimeArray)
	{
		EachBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerWidget::ShowMatchTime()
{
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	
	const float MatchTimeRemain = PlayersArray[0]->MatchTimeRemain;
	
	for(UTextBlock* EachBlock:MatchTimeArray)
	{
		EachBlock->SetVisibility(ESlateVisibility::Visible);
		EachBlock->SetText(FText::AsNumber(MatchTimeRemain,&Opts));
	}
}

void UPlayerWidget::HidePrepareTime()
{
	for(UTextBlock* EachBlock:PrepareTimeArray)
	{
		EachBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerWidget::ShowPrepareTime()
{
	for(UTextBlock* EachBlock:PrepareTimeArray)
	{
		EachBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerWidget::CollectPlayers()
{
	// Find all players in the game world
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);

	// Collect all players
	for(AActor* Actor: Actors)
	{
		if(Cast<AAssignment1BCharacter>(Actor))
		{
			PlayersArray.Add(Cast<AAssignment1BCharacter>(Actor));
		}
	}

	bIsFinishedCollecting = true;
	
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(BasicTimerHandle);
}

void UPlayerWidget::HideWinner()
{
	HidePrepareTime();
	
	PlayersArray[0]->bIsInMatchEnd = false;

	StartTimer = false;
	
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(BasicTimerHandle);
}

void UPlayerWidget::ManagerMatchTime()
{
	if(PlayersArray[0]->bIsInMatch)
	{
		ShowMatchTime();
	}
	else
	{
		HideMatchTime();
	}
}

void UPlayerWidget::ManagerWinner()
{
	if(PlayersArray[0]->bIsInMatchEnd)
	{
		if(!StartTimer)
		{
			StartTimer = true;
			GetWorld()->GetTimerManager().SetTimer(BasicTimerHandle, this, &UPlayerWidget::HideWinner, DisplayWinnerTimeRequired, true);
		}
		ShowPrepareTime();
		FString WinnerTeam;
		FLinearColor WinnerTeamColour;
		switch (PlayersArray[0]->WhichTeamWin)
		{
		case 0:
			{
				WinnerTeam = "Draw";
				WinnerTeamColour = FLinearColor::Black;
				break;
			}
		case 1:
			{
				WinnerTeam = "Winner Red";
				WinnerTeamColour = FLinearColor::Red;
				break;
			}
		case 2:
			{
				WinnerTeam = "Winner Blue";
				WinnerTeamColour = FLinearColor::Blue;
				break;
			}
		default:{}
		}
			
		for(UTextBlock* EachBlock:PrepareTimeArray)
		{
			EachBlock->SetText(FText::FromString(WinnerTeam));
			EachBlock->SetColorAndOpacity(WinnerTeamColour);
		}
	}
}

void UPlayerWidget::ManagePrepareTimeAndCrossHair()
{
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	for(int i = 0; i < PlayersArray.Num(); i++)
	{
		if(PlayersArray[i]->bIsInPrepare)
		{
			const float PrepareTimeRemain = PlayersArray[i]->PrepareTimeRemain;
			PrepareTimeArray[i]->SetVisibility(ESlateVisibility::Visible);
			PrepareTimeArray[i]->SetText(FText::AsNumber(PrepareTimeRemain,&Opts));
		}
		else
		{
			PrepareTimeArray[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		
		CrossHairArray[i]->SetBrushTintColor(PlayersArray[i]->NextCubeColour);
	}
}
