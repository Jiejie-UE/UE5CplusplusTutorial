// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDUserWidget.h"
#include "Components/TextBlock.h"
#include "../MyCharacter/Shooter_PlayerState.h"
#include "GameFramework/GameStateBase.h"

void UInGameHUDUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	All_Text_PlayerName.Add(Text_PlayerName_0);
	All_Text_PlayerName.Add(Text_PlayerName_1);
	All_Text_PlayerName.Add(Text_PlayerName_2);
	All_Text_PlayerName.Add(Text_PlayerName_3);
}

void UInGameHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(NativeConstructTimerHandle, this, &UInGameHUDUserWidget::UpdateInGameHUD, 2.0f, false);
	
}

void UInGameHUDUserWidget::GameStartCountDown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("GameStartCountDown")));
	if (Text_CountDown) {
		//SetVisibility(ESlateVisibility::Visible);
		Text_CountDown->SetText(FText::FromString(FString::FromInt(CountdownValue)));
		Text_CountDown->SetVisibility(ESlateVisibility::Visible);

		GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UInGameHUDUserWidget::CountDownDelayCount, 1.0f, true);
	}
}

void UInGameHUDUserWidget::CountDownDelayCount()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("DelayCount %d"), CountdownValue));
	if (CountdownValue > 0) {
		//3,2,1
		CountdownValue--;
		Text_CountDown->SetText(FText::FromString(FString::FromInt(CountdownValue)));
		return;
	}
	else if (CountdownValue == 0) {
		//0
		// 这里会让这个Round 1显示3秒
		CountdownValue--;
		Text_CountDown->SetText(FText::FromString(TEXT("Round 1")));
		return;
	}
	else if (CountdownValue >= -2) {
		//-1
		CountdownValue--;
		return;
	}
	else if (CountdownValue == -3)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		Text_CountDown->SetVisibility(ESlateVisibility::Hidden);
		//SetVisibility(ESlateVisibility::Hidden);
		//RemoveFromParent();
		return;
	}
}

void UInGameHUDUserWidget::UpdateInGameHUD()
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState()) {
		for (int32 i = 0; i < All_Text_PlayerName.Num(); ++i) {
			if (GameState->PlayerArray.Num() - 1 < i) {
				All_Text_PlayerName[i]->RemoveFromParent();
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT(" GameState->PlayerArray.Num() == %d"), GameState->PlayerArray.Num()));

		for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i) {
			AShooter_PlayerState* PlayerState = Cast<AShooter_PlayerState>(GameState->PlayerArray[i]);
			if (PlayerState) {
				All_Text_PlayerName[i]->SetText(PlayerState->PlayerProfile.PlayerName);
			}
		}
	}
}

