// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"
class UButton;
class UTutorialGameInstance;
class UEditableText;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_SoloGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_HostGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_SearchGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_QuitGame;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UEditableText* Input_PlayerName;

public:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnBttn_QuitGameClick();

	UFUNCTION()
	void OnBttn_SoloGameClick();

	UFUNCTION()
	void OnInput_PlayerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
	virtual void NativeConstruct() override;

	UTutorialGameInstance* MyGameInstance;

};
