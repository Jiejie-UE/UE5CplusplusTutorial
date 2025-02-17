// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelRowInfoUserWidget.generated.h"

class UButton;
class UTextBlock;
class ULobbyMenuUserWidget;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API ULevelRowInfoUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_MapName;
	
	UPROPERTY()
	FText Map_displayName;

	UPROPERTY()
	FName TableRowName;

	UPROPERTY()
	ULobbyMenuUserWidget* WB_LobbyMenu;

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_MapName;

	UFUNCTION()
	void OnBttn_MapNameCilck();

};
