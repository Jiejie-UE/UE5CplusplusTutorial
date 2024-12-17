// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "S_PlayerProfile.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FS_PlayerProfile
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerProfile")
	FText PlayerName;

	// 默认构造函数
	FS_PlayerProfile()
		: PlayerName(FText::FromString(TEXT("Hello Unreal!")))
	{
	}
};
