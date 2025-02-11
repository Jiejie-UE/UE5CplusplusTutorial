// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelRowInfoUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../UserWidget/LobbyMenuUserWidget.h"

void ULevelRowInfoUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Bttn_MapName) {
		Bttn_MapName->OnClicked.AddDynamic(this, &ULevelRowInfoUserWidget::OnBttn_MapNameCilck);
	}

	
}

void ULevelRowInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Text_MapName) {
		Text_MapName->SetText(Map_displayName);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ULevelRowInfoUserWidget::NativeConstruct")));
	}
}

void ULevelRowInfoUserWidget::OnBttn_MapNameCilck()
{
	if (WB_LobbyMenu) {
		WB_LobbyMenu->Server_SelectLevel(TableRowName);
	}
}
