// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UETUTORIAL_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	
};
