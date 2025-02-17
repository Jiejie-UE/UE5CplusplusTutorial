// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "S_PlayerProfile.h"
#include "Shooter_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UETUTORIAL_API AShooter_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	FS_PlayerProfile PlayerProfile;
	
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
