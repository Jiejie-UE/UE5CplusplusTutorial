// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "S_PlayerProfile.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online.h" 
#include "TutorialGameInstance.generated.h"

class UUserWidget;
class USG_PlayerProfile;
class UMainMenuUserWidget;
class UMPUserWidget;
class FOnlineSessionSearchResult;
class UServerRowUserWidget;
class ULobbyMenuUserWidget;

DECLARE_MULTICAST_DELEGATE(FCreateMPSessionEventDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FJoinMPSessionEventDelegate,FOnlineSessionSearchResult&);
//1.4.2.3
DECLARE_MULTICAST_DELEGATE_TwoParams(FTravelToMapEventDelegate,int32,FString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartSoloGameEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChange_PlayerNameEventDelegate, FText, PlayerName);

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

	//---------------------------
	UFUNCTION()
	void UI_ShowMpMenu();

	//记得在编辑器中设置
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMPUserWidget> MPUserWidgetClass;

	UPROPERTY()
	UMPUserWidget* MPMenu;

	UPROPERTY(EditAnywhere)
	bool bIsLanConnection;

	UFUNCTION()
	bool ChangeConnectionType();

	FCreateMPSessionEventDelegate OnCreateSession;

	void TriggerSessionCreation();

	UFUNCTION()
	void OnCreateMPSession();

	void CreateLocalSession(int32 NumPublicConnections);

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void StartLocalSessionCreation(int32 NumPublicConnections);

	FJoinMPSessionEventDelegate OnJoinSession;

	void TriggerSessionJoin(FOnlineSessionSearchResult& SessionResult);

	//UFUNCTION()
	void OnJoinMPSession(FOnlineSessionSearchResult& SessionResult);

	UPROPERTY(EditAnywhere)
	int32 MaxNumPublicConnections = 2;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UServerRowUserWidget> ServerRowUserWidgetClass;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void ChangeIsLanConnectionBySubsystemName();

	//1.4.2.3
	FName CurrentSessionName;

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FTravelToMapEventDelegate TravelToMapEventDelegate;

	void TriggerTravelToMap(int32 NumConnectdPlayers, FString MapName);

	void TravelToMap(int32 NumConnectdPlayers, FString MapName);

	UPROPERTY()
	int32 MP_NumConnectedPlayers;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<ULobbyMenuUserWidget> LobbyMenuUserWidgetClass;


};
