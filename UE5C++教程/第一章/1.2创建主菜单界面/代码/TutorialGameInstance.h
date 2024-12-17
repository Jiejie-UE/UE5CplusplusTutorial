// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "S_PlayerProfile.h"

#include "TutorialGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartSoloGameEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChange_PlayerNameEventDelegate, FText, PlayerName);

class UUserWidget;
class USG_PlayerProfile;
class UMainMenuUserWidget;


/**
 * 
 */
UCLASS()
class UETUTORIAL_API UTutorialGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	
public:
	UFUNCTION()
	void UI_ShowMainMenu();
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMainMenuUserWidget> MainMenuWidgetClass;
	
	UPROPERTY()
	UMainMenuUserWidget* MainMenu;

	bool IsSoloGame;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStartSoloGameEventDelegate OnStartSoloGameEventTriggered;

	void TriggerStartSoloGameEvent();

	UFUNCTION()
	void StartSoloGameEvent();

	UFUNCTION()
	void CheckForSavedProfile();

	FString PlayerProflie_Shot = FString("PlayerProflie_Shot");

	UFUNCTION()
	void LoadProfile();

	UPROPERTY()
	FS_PlayerProfile PlayerProfileinfo;

	UFUNCTION()
	void SaveProfile();

	UPROPERTY()
	USG_PlayerProfile* SG_PlayerProfile;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FChange_PlayerNameEventDelegate ChangePlayerNameEvent;

	UFUNCTION()
	void OnPlayerNameChanged(FText PlayerName);

	void TriggerPlayerNameChangedEvent(FText PlayerName);
};
