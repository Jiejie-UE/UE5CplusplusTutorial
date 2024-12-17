// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "TutorialGameInstance.h"

void AMainMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	if (MyGameInstance) {
		MyGameInstance->UI_ShowMainMenu();
	}

}
