// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Delegates/Delegate.h"
#include "SG_PlayerProfile.h"
#include "MainMenuUserWidget.h"
#include "Components/EditableText.h"
#include "MPUserWidget.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerState.h"
#include "../UserWidget/LobbyMenuUserWidget.h"
#include "../UserWidget/PauseMenuUserWidget.h"
#include "ShooterPlayerController.h"

void UTutorialGameInstance::Init()
{
	Super::Init();

	OnStartSoloGameEventTriggered.AddDynamic(this, &UTutorialGameInstance::StartSoloGameEvent);
	ChangePlayerNameEvent.AddDynamic(this, &UTutorialGameInstance::OnPlayerNameChanged);
	OnCreateSession.AddUObject(this ,&UTutorialGameInstance::OnCreateMPSession);
	OnJoinSession.AddUObject(this, &UTutorialGameInstance::OnJoinMPSession);

	//1.4.2.3
	TravelToMapEventDelegate.AddUObject(this, &UTutorialGameInstance::TravelToMap);

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get("")) {
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// 绑定 OnCreateSessionComplete 委托
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTutorialGameInstance::OnCreateSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTutorialGameInstance::OnJoinSessionComplete);
			//1.4.2.3
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTutorialGameInstance::OnDestroySessionComplete);		
		}
	}
	
}

void UTutorialGameInstance::UI_ShowMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("UI_ShowMainMenu is entering"));

	if (!IsValid(MainMenu)) {
		MainMenu = CreateWidget<UMainMenuUserWidget>(GetWorld(), MainMenuWidgetClass);
	}
	MainMenu->AddToViewport(0);
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}

	CheckForSavedProfile();

	MainMenu->Input_PlayerName->SetText(PlayerProfileinfo.PlayerName);

}

void UTutorialGameInstance::TriggerStartSoloGameEvent()
{
	OnStartSoloGameEventTriggered.Broadcast();
}

void UTutorialGameInstance::StartSoloGameEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("StartSoloGameEvent is entering"));
	IsSoloGame = true;
	UGameplayStatics::OpenLevel(this, FName("Level01"));
}

void UTutorialGameInstance::CheckForSavedProfile()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering CheckForSavedProfile"));
	if (UGameplayStatics::DoesSaveGameExist(PlayerProflie_Shot, 0)) {
		LoadProfile();
	}
	else {
		SaveProfile();
	}
}

void UTutorialGameInstance::LoadProfile()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering LoadProfile"));

	SG_PlayerProfile = Cast<USG_PlayerProfile>(UGameplayStatics::LoadGameFromSlot(PlayerProflie_Shot, 0));
	if (SG_PlayerProfile) {
		UE_LOG(LogTemp, Warning, TEXT("SG_PlayerProfile is true"));
		UE_LOG(LogTemp, Warning, TEXT("SG_PlayerProfile name : %s"), *(SG_PlayerProfile->PlayerProfileStruct.PlayerName).ToString());
		PlayerProfileinfo = SG_PlayerProfile->PlayerProfileStruct;
		UE_LOG(LogTemp, Warning, TEXT("PlayerProfileinfo name : %s"), *(PlayerProfileinfo.PlayerName).ToString());
	}
}

void UTutorialGameInstance::SaveProfile()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering SaveProfile"));
	if (IsValid(SG_PlayerProfile)) {
		UE_LOG(LogTemp, Warning, TEXT("SG_PlayerProfile IsValid"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SG_PlayerProfile IsNotValid"));
		SG_PlayerProfile = Cast<USG_PlayerProfile>(UGameplayStatics::CreateSaveGameObject(USG_PlayerProfile::StaticClass()));
	}
	if (SG_PlayerProfile) {
		UE_LOG(LogTemp, Warning, TEXT("SaveProfile(): PlayerProfileinfo name : %s"), *(PlayerProfileinfo.PlayerName).ToString());
		SG_PlayerProfile->PlayerProfileStruct = PlayerProfileinfo;
		bool bTemp = UGameplayStatics::SaveGameToSlot(SG_PlayerProfile, PlayerProflie_Shot, 0);
		UE_LOG(LogTemp, Warning, TEXT("SaveProfile Success: %s"), bTemp ? TEXT("true") : TEXT("false"));

	}
}

void UTutorialGameInstance::OnPlayerNameChanged(FText PlayerName)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerNameChanged is entering"));
	
	UE_LOG(LogTemp, Warning, TEXT("Player name changed to: %s"), *PlayerName.ToString());

	PlayerProfileinfo.PlayerName = PlayerName;
	SaveProfile();
}

void UTutorialGameInstance::TriggerPlayerNameChangedEvent(FText PlayerName)
{
	ChangePlayerNameEvent.Broadcast(PlayerName);
}

void UTutorialGameInstance::UI_ShowMpMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("UI_ShowMpMenu is entering"));

	if (!IsValid(MPMenu)) {
		MPMenu = CreateWidget<UMPUserWidget>(GetWorld(), MPUserWidgetClass);
	}
	MPMenu->AddToViewport(0);
	
}

bool UTutorialGameInstance::ChangeConnectionType()
{
	bIsLanConnection = !bIsLanConnection;

	return bIsLanConnection;
}

void UTutorialGameInstance::TriggerSessionCreation()
{
	OnCreateSession.Broadcast();
}

void UTutorialGameInstance::OnCreateMPSession()
{
	IsSoloGame = false;
	StartLocalSessionCreation(MaxNumPublicConnections);
}

void UTutorialGameInstance::CreateLocalSession(int32 NumPublicConnections)
{
	
	if (SessionInterface.IsValid()) {
		FName SessionName (*PlayerProfileinfo.PlayerName.ToString());

		// 配置会话设置
		FOnlineSessionSettings SessionSettings;
		
		SessionSettings.bIsLANMatch = bIsLanConnection;                       // 使用局域网
		if (bIsLanConnection) {
			UE_LOG(LogTemp, Warning, TEXT("bIsLanConnection is ture"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("bIsLanConnection is ture")));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("bIsLanConnection is false"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("bIsLanConnection is false")));
		}
		SessionSettings.bShouldAdvertise = true;                  // 广播会话信息
		SessionSettings.NumPublicConnections = NumPublicConnections; // 最大玩家数
		SessionSettings.bAllowJoinInProgress = true;              // 允许加入进行中的会话
		SessionSettings.bUsesPresence = true;                    // 不使用在线状态
		SessionSettings.bIsDedicated = false; // 设置为非专用服务器,
		SessionSettings.bAllowJoinViaPresence = true;	//...
		SessionSettings.bUseLobbiesIfAvailable = true;	//...
			
		SessionSettings.Set(FName(TEXT("SERVER_NAME")), PlayerProfileinfo.PlayerName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		//1.4.1
		if (!bIsLanConnection) {//bIsLanConnection==false, Online
			SessionSettings.BuildUniqueId = rand();
		}
		

		// 创建会话
		bool bResult = SessionInterface->CreateSession(0, SessionName, SessionSettings);

		if (bResult) {
			UE_LOG(LogTemp, Warning, TEXT("Session creation started successfully."));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Session creation started successfully.")));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to initiate session creation."));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Failed to initiate session creation.")));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Session interface is invalid."));
	}
	
	
}

void UTutorialGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Session %s created successfully!"), *SessionName.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("Session %s created successfully!"), *SessionName.ToString());
		//UGameplayStatics::OpenLevel(this, FName("LobbyMenu_Map"), false, TEXT("listen"));

		//1.4.2.3
		CurrentSessionName = SessionName;

		GetWorld()->ServerTravel("LobbyMenu_Map?listen");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create session %s!"), *SessionName.ToString());
	}
}

void UTutorialGameInstance::StartLocalSessionCreation(int32 NumPublicConnections)
{
	CreateLocalSession(NumPublicConnections);  // 创建会话
}

void UTutorialGameInstance::TriggerSessionJoin(FOnlineSessionSearchResult& SessionResult)
{
	OnJoinSession.Broadcast(SessionResult);
}

void UTutorialGameInstance::OnJoinMPSession(FOnlineSessionSearchResult& SessionResult)
{
	FString ServerName;
	SessionResult.Session.SessionSettings.Get(FName(TEXT("SERVER_NAME")), ServerName);

	FName SessionName(*ServerName);

	IsSoloGame = false;
	SessionInterface->JoinSession(0, SessionName, SessionResult);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("JoinSession successfully! %s"), *ServerName));

}

void UTutorialGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "") {
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("In OnJoinSessionComplete! %s!%s!%d"), *JoinAddress,*SessionName.ToString(), Result));
		//1.4.2.3
		CurrentSessionName = SessionName;
	}
	
}

void UTutorialGameInstance::ChangeIsLanConnectionBySubsystemName()
{
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") {
		bIsLanConnection = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Steam")));
	}
	else {
		bIsLanConnection = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("NULL")));
	}
	
}

//1.4.2.3
void UTutorialGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		
		CurrentSessionName = FName();

		if (WillQuitGame) {
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OnDestroySessionComplete bWasSuccessful is true! %s!"), *SessionName.ToString()));
			UGameplayStatics::OpenLevel(this, FName("MainMenu_Map"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OnDestroySessionComplete bWasSuccessful is false! %s!"),*SessionName.ToString()));
	}
}

void UTutorialGameInstance::TriggerTravelToMap(int32 NumConnectdPlayers, FString MapName)
{
	TravelToMapEventDelegate.Broadcast(NumConnectdPlayers, MapName);
}

void UTutorialGameInstance::TravelToMap(int32 NumConnectdPlayers, FString MapName)
{
	MP_NumConnectedPlayers = NumConnectdPlayers;
	///Travel
	GetWorld()->ServerTravel(MapName + "?listen", true);
}

void UTutorialGameInstance::UI_ShowPauseMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("UI_ShowPauseMenu")));
	if (!IsValid(PauseMenu)) {
		PauseMenu = CreateWidget<UPauseMenuUserWidget>(GetWorld(), PauseMenuWidgetClass);
	}
	PauseMenu->AddToViewport(0);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}

}

bool UTutorialGameInstance::TogglePauseGame()
{
	bool IsGamePausedResult = false;
	if (IsSoloGame) {
		//取反
		if (UGameplayStatics::IsGamePaused(GetWorld())) {
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			IsGamePausedResult = false;
			PauseMenu->RemoveFromParent();
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
				AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(PlayerController);
				if (ShooterPlayerController) {
					ShooterPlayerController->UnHide_IngameHUD();
				}
			}
		}
		else {
			UGameplayStatics::SetGamePaused(GetWorld(),true);
			IsGamePausedResult = true;
			UI_ShowPauseMenu();
		}
	}
	else {
		if (MP_IsPauseMenuVisable) {
			MP_IsPauseMenuVisable = false;
			IsGamePausedResult = false;
			PauseMenu->RemoveFromParent();
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
				AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(PlayerController);
				if (ShooterPlayerController) {
					ShooterPlayerController->UnHide_IngameHUD();
				}
			}
		}
		else {
			MP_IsPauseMenuVisable = true;
			IsGamePausedResult = true;
			UI_ShowPauseMenu();
		}
	}

	return IsGamePausedResult;
}







