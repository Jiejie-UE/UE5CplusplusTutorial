// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_ResumeGame;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_MainMenu;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_QuitGame;
	UFUNCTION()
	void On_Bttn_ResumeGameClick();
	UFUNCTION()
	void On_Bttn_MainMenuClick();
	UFUNCTION()
	void Bttn_QuitGameClick();
};
