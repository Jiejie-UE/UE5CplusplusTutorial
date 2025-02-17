// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UETUTORIAL_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	//1.6
	void UI_ShowInGameHUD();

	UPROPERTY()
	UInGameHUDUserWidget* InGameHUD;

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void EnableAllInput();
	void EnableAllInput_Implementation();

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void GameStartCountdown();
	void GameStartCountdown_Implementation();

	UFUNCTION()
	void UnHide_IngameHUD();
	
	UFUNCTION(Client, Reliable, Category = "Lobby")
	void DestroyClientSession();
	void DestroyClientSession_Implementation();

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void UpdateInGameHUD();
	void UpdateInGameHUD_Implementation();
};
