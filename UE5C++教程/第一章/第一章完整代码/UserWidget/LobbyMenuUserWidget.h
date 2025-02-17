// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MyCharacter/S_PlayerProfile.h"
#include "LobbyMenuUserWidget.generated.h"

class UButton;
class UTutorialGameInstance;
class UTextBlock;
class UBorder;
class UCheckBox;
class ULevelRowInfoUserWidget;
class UScrollBox;
class UImage;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API ULobbyMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_MainMenu;

	UFUNCTION()
	void OnBttn_MainMenuClick();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* Bttn_StartGame;

	UFUNCTION()
	void OnBttn_StartGameCilck();
	
	UTutorialGameInstance* MyGameInstance;

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_PlayerName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_PlayerName_1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_PlayerName_2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_PlayerName_3;
	UPROPERTY()
	TArray<UBorder*> All_Border_PlayerName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName_3;
	UPROPERTY()
	TArray<UTextBlock*> All_Text_PlayerName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_StartGameBttn;
	
	UFUNCTION()
	void UpdatePlayerNames(const TArray<FS_PlayerProfile>& PlayerProfiles);

	UFUNCTION()
	void ClearPlayerNames();

	//1.5.1
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCheckBox* CheckBox_SeamlessTravel;

	UFUNCTION()
	void OnCheckBox_SeamlessTravelChanged(bool IsClick);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBorder* Border_SeamlessTravel;

	UPROPERTY()
	FString SelectedLevel_String;

	UPROPERTY()
	TArray<ULevelRowInfoUserWidget*> All_LevelInfoRows;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UScrollBox* ScrollBox_AllLevelName;

	UPROPERTY()
	UDataTable* LevelInfo_Table;

	UFUNCTION()
	void UpdatelevelSelection(FName LevelRowName);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_SelectedMapName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UImage* Image_PreviewImage;

	UFUNCTION()
	void Server_SelectLevel(FName LevelRowName);
};
