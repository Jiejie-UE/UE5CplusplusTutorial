// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) {
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		//1.5.1
		//if (GEngine && GEngine->GameViewport) {
		//	GEngine->GameViewport->RemoveAllViewportWidgets();
		//}

	}

}
