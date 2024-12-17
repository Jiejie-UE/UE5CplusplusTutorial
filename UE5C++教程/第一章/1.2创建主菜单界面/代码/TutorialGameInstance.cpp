// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Delegates/Delegate.h"
#include "SG_PlayerProfile.h"
#include "MainMenuUserWidget.h"
#include "Components/EditableText.h"

void UTutorialGameInstance::Init()
{
	Super::Init();

	OnStartSoloGameEventTriggered.AddDynamic(this, &UTutorialGameInstance::StartSoloGameEvent);
	ChangePlayerNameEvent.AddDynamic(this, &UTutorialGameInstance::OnPlayerNameChanged);

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
