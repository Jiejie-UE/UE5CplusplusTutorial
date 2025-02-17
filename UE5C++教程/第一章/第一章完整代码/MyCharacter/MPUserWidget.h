// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online.h" 
#include "TimerManager.h"
#include "MPUserWidget.generated.h"

class UButton;
class UTextBlock;
class UBorder;
class UTutorialGameInstance;
class UScrollBox;
class UServerRowUserWidget;


/**
 * 
 */
UCLASS()
class UETUTORIAL_API UMPUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

protected:
	virtual void NativeConstruct() override;

	UTutorialGameInstance* MyGameInstance;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_MainMenu;

	UFUNCTION()
	void OnBttn_MainMenuClick();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Connecttype;

	UFUNCTION()
	void SetConnectionType_Text(bool IsLanConnenction);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_HostGame;

	UFUNCTION()
	void OnBttn_HostGameClick();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_MenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_ServerList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_SearchGame;
	
	UFUNCTION()
	void OnBttn_SearchGameClick();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UScrollBox* ScrollBox_ServerList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TArray<UServerRowUserWidget*> AllServerinfo_WB;

	UFUNCTION()
	void ClearAllSeverinfo_WB();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_SearchGameButton;

	void OnFindSessionsComplete(bool bWassuccessful);

	//1.4.2
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_ConnectionType;

	//1.4.2
	UFUNCTION()
	void OnBttn_ConnectionTypeClick();

};
