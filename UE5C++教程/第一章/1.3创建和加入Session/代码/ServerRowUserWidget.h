// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowUserWidget.generated.h"
class FOnlineSessionSearchResult;
class UMPUserWidget;
class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateSeverRowInfoEventDelegate, FOnlineSessionSearchResult&);

/**
 * 
 */
UCLASS()
class UETUTORIAL_API UServerRowUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	FOnlineSessionSearchResult* Result;

	UPROPERTY()
	UMPUserWidget* WB_MpMenu;
	
	FUpdateSeverRowInfoEventDelegate UpdateSeverRowInfoEvent;

	void TriggerUpdateServerRowInfo(FOnlineSessionSearchResult& SessionResult);

private:
	
	void UpdateServerRow_Info(FOnlineSessionSearchResult& SessionResult);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_ServerName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_NumPlayer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Ping;

	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_JoinGame;

	UFUNCTION()
	void JoinGame();

};
