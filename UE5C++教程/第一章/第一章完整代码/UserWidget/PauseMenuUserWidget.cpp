// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Components/Button.h"
#include "../MyCharacter/TutorialGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../MyCharacter/TutorialGameMode.h"
#include "../MyCharacter/ShooterPlayerController.h"

void UPauseMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Bttn_ResumeGame) {
		Bttn_ResumeGame->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::On_Bttn_ResumeGameClick);
	}
	if (Bttn_MainMenu) {
		Bttn_MainMenu->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::On_Bttn_MainMenuClick);
	}
	if (Bttn_QuitGame) {
		Bttn_QuitGame->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Bttn_QuitGameClick);
	}
}

void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPauseMenuUserWidget::On_Bttn_ResumeGameClick()
{
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	if (MyGameInstance) {
		MyGameInstance->TogglePauseGame();
	}
}

void UPauseMenuUserWidget::On_Bttn_MainMenuClick()
{
	//这是在多人下的逻辑，单人需要额外写，单人不需要DestroyClientSession
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnBttn_MainMenuClick"));
	AShooterPlayerController* MyController = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyController && MyController->HasAuthority()) {
		//OnServer
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer"));
		ATutorialGameMode* GameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			for (AShooterPlayerController* Controller : GameMode->All_ConnectedControllers) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer :GameMode for"));
				if (Controller != MyController) {
					Controller->DestroyClientSession();
				}
			}
			UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
			if (MyGameInstance) {
				MyGameInstance->MP_IsPauseMenuVisable = false;
			}
			MyController->DestroyClientSession();
		}
	}
	else {
		//OnClient
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnClient"));
		if (MyController)
		{
			UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
			if (MyGameInstance) {
				MyGameInstance->MP_IsPauseMenuVisable = false;
			}
			MyController->DestroyClientSession();
		}
	}
}

void UPauseMenuUserWidget::Bttn_QuitGameClick()
{
	//这是在多人下的逻辑，单人需要额外写，单人不需要DestroyClientSession
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnBttn_MainMenuClick"));
	AShooterPlayerController* MyController = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyController && MyController->HasAuthority()) {
		//OnServer
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer"));
		ATutorialGameMode* GameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			for (AShooterPlayerController* Controller : GameMode->All_ConnectedControllers) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer :GameMode for"));
				if (Controller != MyController) {
					Controller->DestroyClientSession();
				}
			}
			UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
			if (MyGameInstance) {
				MyGameInstance->MP_IsPauseMenuVisable = false;
				MyGameInstance->WillQuitGame = true;
			}
			MyController->DestroyClientSession();
		}
	}
	else {
		//OnClient
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnClient"));
		if (MyController)
		{
			UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
			if (MyGameInstance) {
				MyGameInstance->MP_IsPauseMenuVisable = false;
				MyGameInstance->WillQuitGame = true;
			}
			MyController->DestroyClientSession();
		}
	}
}
