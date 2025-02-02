// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootMenuGameMode.generated.h"

class AShootMenuController;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API AShootMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<AShootMenuController*> All_ControllerPlayers;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void UpdateLobby(bool PlayerNames);
	
	UPROPERTY()
	TArray<FS_PlayerProfile> All_PlayerProfiles;
};
