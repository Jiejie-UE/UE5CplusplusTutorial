// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUDUserWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UETUTORIAL_API UInGameHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
	void GameStartCountDown();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_CountDown;
	
	UFUNCTION()
	void CountDownDelayCount();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Countdown")
	int32 CountdownValue = 3;

	UPROPERTY()
	FTimerHandle CountDownTimerHandle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_0;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_3;
	UPROPERTY()
	TArray<UTextBlock*> All_Text_PlayerName;

	UPROPERTY()
	FTimerHandle NativeConstructTimerHandle;

	UFUNCTION()
	void UpdateInGameHUD();

};
