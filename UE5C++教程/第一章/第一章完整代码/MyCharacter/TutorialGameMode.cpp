// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialGameMode.h"
#include "ShooterPlayerController.h"
#include "TutorialGameInstance.h"


void ATutorialGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ATutorialGameMode::HandleStartingNewPlayer_Implementation")));
	AShooterPlayerController* NewController = Cast<AShooterPlayerController>(NewPlayer);
	if (NewController) {
		All_ConnectedControllers.Add(NewController);
	}
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	if (MyGameInstance) {
		if (MyGameInstance->IsSoloGame) {
			Num_ExpectedPlayer = 1;
			RestartPlayerAtPlayerStart(All_ConnectedControllers[0],FindPlayerStart(All_ConnectedControllers[0]));
			LevelStart();
		}
		else {
			Num_ExpectedPlayer = MyGameInstance->MP_NumConnectedPlayers;
			if (Num_ExpectedPlayer == All_ConnectedControllers.Num()) {
				GetWorld()->GetTimerManager().SetTimer(LevelStartTimerHandle, this, &ATutorialGameMode::HandleStartingNewPlayerDelay1, 0.5f, false);
			}
		}
	}
}

void ATutorialGameMode::HandleStartingNewPlayerDelay1()
{
	for (int32 i = 0; i < All_ConnectedControllers.Num(); ++i) {
		RestartPlayerAtPlayerStart(All_ConnectedControllers[i], FindPlayerStart(All_ConnectedControllers[i]));
	}
	LevelStart();
}

void ATutorialGameMode::Logout(AController* Exiting)
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(Exiting);
	if (PlayerController) {
		All_ConnectedControllers.Remove(PlayerController);
		Num_ExpectedPlayer--;
		GetWorld()->GetTimerManager().SetTimer(LogoutTimerHandle, this, &ATutorialGameMode::LogoutDelay1, 0.5f, false);

	}
}

void ATutorialGameMode::LogoutDelay1()
{
	for (int32 i = 0; i < All_ConnectedControllers.Num(); ++i) {
		All_ConnectedControllers[i]->UpdateInGameHUD();
	}
}

void ATutorialGameMode::LevelStart()
{
	GetWorld()->GetTimerManager().SetTimer(LevelStartTimerHandle, this, &ATutorialGameMode::LevelStartDelay1, 0.2f, false);
}
void ATutorialGameMode::LevelStartDelay1()
{
	for (int32 i = 0; i < All_ConnectedControllers.Num(); ++i) {
		All_ConnectedControllers[i]->GameStartCountdown();
	}

	GetWorld()->GetTimerManager().SetTimer(LevelStartTimerHandle, this, &ATutorialGameMode::LevelStartDelay2, 3.0f, false);
}
void ATutorialGameMode::LevelStartDelay2()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("LevelStartDelay2")));
	for (int32 i = 0; i < All_ConnectedControllers.Num(); ++i) {
		All_ConnectedControllers[i]->EnableAllInput();
	}
}


