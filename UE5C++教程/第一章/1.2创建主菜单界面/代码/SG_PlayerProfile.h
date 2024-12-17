// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "S_PlayerProfile.h"
#include "SG_PlayerProfile.generated.h"

struct FS_PlayerProfile;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API USG_PlayerProfile : public USaveGame
{
	GENERATED_BODY()
	
public:
	USG_PlayerProfile();

	UPROPERTY()
	FS_PlayerProfile PlayerProfileStruct;

	
};
