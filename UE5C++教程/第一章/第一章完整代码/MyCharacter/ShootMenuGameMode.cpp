// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootMenuGameMode.h"
#include "ShootMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter_PlayerState.h"

void AShootMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Player Joined!"));

    FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
    if (LevelName.Equals(TEXT("LobbyMenu_Map")))
    {
        AShootMenuController* NewPlayerController = Cast<AShootMenuController>(NewPlayer);
        if (NewPlayerController)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NewPlayerController"));
            All_ControllerPlayers.Add(NewPlayerController);
            NewPlayerController->ClientCreateLobbyMenu();
            NewPlayerController->Init_Setup();
        }
    }
}

void AShootMenuGameMode::UpdateLobby(bool PlayerNames, bool LevelSelection)
{
    All_PlayerProfiles.Empty();
    for (int32 i = 0; i < All_ControllerPlayers.Num(); ++i) {
        All_PlayerProfiles.Add(All_ControllerPlayers[i]->PlayerProfileinfo);
        AShooter_PlayerState* PlayerState = All_ControllerPlayers[i]->GetPlayerState<AShooter_PlayerState>();
        if (PlayerState) {
            PlayerState->PlayerProfile = All_ControllerPlayers[i]->PlayerProfileinfo;
        }
    }

    if (PlayerNames) {
        for (int32 i = 0; i < All_ControllerPlayers.Num(); ++i) {
            All_ControllerPlayers[i]->UpdatePlayerNames(All_PlayerProfiles);
        }
    }
    if (LevelSelection) {
        for (int32 i = 0; i < All_ControllerPlayers.Num(); ++i) {
            All_ControllerPlayers[i]->UpdateLevelSelection(SelectedLevelName);
        }
    }
}

void AShootMenuGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    AShootMenuController* ExitPlayerController = Cast<AShootMenuController>(Exiting);
    if (ExitPlayerController) {
        All_ControllerPlayers.Remove(ExitPlayerController);
        UpdateLobby(true, true);
    }

}
