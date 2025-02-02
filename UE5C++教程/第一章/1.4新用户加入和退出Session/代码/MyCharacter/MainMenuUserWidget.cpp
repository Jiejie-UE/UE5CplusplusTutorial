// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/EditableText.h"
#include "TutorialGameInstance.h"

void UMainMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Bttn_QuitGame) {
		Bttn_QuitGame->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnBttn_QuitGameClick);
	}
	if (Bttn_SoloGame) {
		Bttn_SoloGame->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnBttn_SoloGameClick);
	}
	if (Input_PlayerName) {
		Input_PlayerName->OnTextCommitted.AddDynamic(this, &UMainMenuUserWidget::OnInput_PlayerNameCommitted);
	}
	if (Bttn_MPGame) {
		Bttn_MPGame->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnBttn_MPGameClick);
	}
}

void UMainMenuUserWidget::OnBttn_QuitGameClick()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	}
}

void UMainMenuUserWidget::OnBttn_SoloGameClick()
{
	if (MyGameInstance) {
		MyGameInstance->TriggerStartSoloGameEvent();
	}
}

void UMainMenuUserWidget::OnInput_PlayerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	MyGameInstance->OnPlayerNameChanged(Text);
}

void UMainMenuUserWidget::OnBttn_MPGameClick()
{
	MyGameInstance->UI_ShowMpMenu();
	RemoveFromParent();
}

void UMainMenuUserWidget::NativeConstruct()
{	
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called"));

	MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	
}


