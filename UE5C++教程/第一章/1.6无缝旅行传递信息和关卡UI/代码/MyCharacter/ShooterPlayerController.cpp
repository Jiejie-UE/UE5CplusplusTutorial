// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TutorialGameInstance.h"
#include "../UserWidget/InGameHUDUserWidget.h"
#include "TutorialCharacter.h"
#include "EnhancedInputSubsystems.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController()) {
        UI_ShowInGameHUD();
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
}

void AShooterPlayerController::UI_ShowInGameHUD()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("UI_ShowInGameHUD")));
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	if (MyGameInstance) {
		if (!IsValid(InGameHUD)) {
			InGameHUD = CreateWidget<UInGameHUDUserWidget>(GetWorld(), MyGameInstance->InGameHUDWidgetClass);
		}
        if (InGameHUD) {
            InGameHUD->AddToViewport(0);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("UI_ShowInGameHUD right")));
        }
	}
}

void AShooterPlayerController::EnableAllInput_Implementation()
{
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ATutorialCharacter* MyCharacter = Cast<ATutorialCharacter>(ControlledPawn);
        if (MyCharacter)
        {
            MyCharacter->EnableInput(this);
        }
    }
}

void AShooterPlayerController::GameStartCountdown_Implementation()
{
    if (InGameHUD) {
        InGameHUD->GameStartCountDown();
    }
}

void AShooterPlayerController::UnHide_IngameHUD()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("UnHide_IngameHUD")));
    InGameHUD->SetVisibility(ESlateVisibility::Visible);
}

void AShooterPlayerController::DestroyClientSession_Implementation()
{
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
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

void AShooterPlayerController::UpdateInGameHUD_Implementation()
{
	if (InGameHUD) {
		InGameHUD->UpdateInGameHUD();
	}
}
