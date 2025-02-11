// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootMenuController.h"
#include "TutorialGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online.h" 
#include "../UserWidget/LobbyMenuUserWidget.h"
#include "../UserWidget/LoadScreenUserWidget.h"
#include "ShootMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

void AShootMenuController::BeginPlay()
{
	Super::BeginPlay();

}

void AShootMenuController::DestroyClientSession_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DestroyClientSession"));
	if (MyGameInstance->CurrentSessionName.IsNone()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DestroyClientSession:CurrentSessionName.IsNone()"));

		return;
	}
	if (MyGameInstance->SessionInterface.IsValid()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DestroyClientSession:SessionInterface.IsValid()"));
		MyGameInstance->SessionInterface->DestroySession(MyGameInstance->CurrentSessionName);
	}
}

void AShootMenuController::CreateLobbyMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CreateLobbyMenu"));
	if (IsLocalController()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CreateLobbyMenu:IsLocalController"));
		MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
		if (MyGameInstance) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CreateLobbyMenu:MyGameInstance"));
			if (!IsValid(LobbyMenu)) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("!IsValid(LobbyMenu)"));
				LobbyMenu = CreateWidget<ULobbyMenuUserWidget>(GetWorld(), MyGameInstance->LobbyMenuUserWidgetClass);
			}
			LobbyMenu->AddToViewport(0);

			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		
		}
	}
}

void AShootMenuController::ClientCreateLobbyMenu_Implementation() {
	CreateLobbyMenu();
}

void AShootMenuController::LoadSavedProfile()
{
	if (MyGameInstance) {
		PlayerProfileinfo = MyGameInstance->PlayerProfileinfo;
	}
}

void AShootMenuController::Init_Setup_Implementation()
{
	LoadSavedProfile();
	Server_UpdateLobby(PlayerProfileinfo);
}

void AShootMenuController::UpdatePlayerNames_Implementation(const TArray<FS_PlayerProfile>& PlayerProfiles)
{
	if (IsValid(LobbyMenu)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("UpdatePlayerNames_Implementation"));
		LobbyMenu->ClearPlayerNames();
		LobbyMenu->UpdatePlayerNames(PlayerProfiles);
	}
}

void AShootMenuController::Server_UpdateLobby_Implementation(FS_PlayerProfile PlayerProfile)
{
	PlayerProfileinfo = PlayerProfile;
	AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->UpdateLobby(true,true);
}

void AShootMenuController::CreateLoadScreen()
{
	if (IsLocalController()) {
		if (GEngine && GEngine->GameViewport) {
			GEngine->GameViewport->RemoveAllViewportWidgets();
		}
		if (!MyGameInstance) {
			MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
		}
		if (MyGameInstance) {
			if (!IsValid(LoadScreen)) {
				LoadScreen = CreateWidget<ULoadScreenUserWidget>(GetWorld(), MyGameInstance->LoadScreenUserWidgetClass);
			}
			LoadScreen->AddToViewport(0);

		}
	}
}

void AShootMenuController::ClientCreateLoadScreen_Implementation()
{
	CreateLoadScreen();
}

void AShootMenuController::UpdateLevelSelection_Implementation(FName LevelRowName)
{
	if (IsValid(LobbyMenu)) {
		LobbyMenu->UpdatelevelSelection(LevelRowName);
	}
}






