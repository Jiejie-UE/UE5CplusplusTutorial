// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

class AShooterPlayerController;
class UTutorialGameInstance;

/**
 * 
 */
UCLASS()
class UETUTORIAL_API ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()
	//1.6
public:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	TArray<AShooterPlayerController*> All_ConnectedControllers;

	UPROPERTY()
	int32 Num_ExpectedPlayer;
	
	UFUNCTION()
	void LevelStart();

	UPROPERTY()
	FTimerHandle LevelStartTimerHandle;

	UFUNCTION()
	void LevelStartDelay1();

	UFUNCTION()
	void LevelStartDelay2();

	UFUNCTION()
	void HandleStartingNewPlayerDelay1();

	virtual void Logout(AController* Exiting) override;

	UPROPERTY()
	FTimerHandle LogoutTimerHandle;

	UFUNCTION()
	void LogoutDelay1();
};
