// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_LevelInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FS_LevelInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerProfile")
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerProfile")
	FString TravelString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerProfile")
	UTexture2D* PreviewImage;

	FS_LevelInfo()
		: DisplayName(FText::FromString(TEXT("DefaultDisplayName")))
		, TravelString(TEXT("DefaultTravelString"))
		, PreviewImage(nullptr)
	{
	}
};
