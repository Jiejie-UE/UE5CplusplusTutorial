// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenuUserWidget.h"
#include "Components/Button.h"
#include "../MyCharacter/ShootMenuController.h"
#include "../MyCharacter/ShootMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../MyCharacter/TutorialGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


void ULobbyMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Bttn_MainMenu) {
		Bttn_MainMenu->OnClicked.AddDynamic(this, &ULobbyMenuUserWidget::OnBttn_MainMenuClick);
	}
	if (Bttn_StartGame) {
		Bttn_StartGame->OnClicked.AddDynamic(this, &ULobbyMenuUserWidget::OnBttn_StartGameCilck);
	}

	MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());

	All_Border_PlayerName.Add(Border_PlayerName);
	All_Border_PlayerName.Add(Border_PlayerName_1);
	All_Border_PlayerName.Add(Border_PlayerName_2);
	All_Border_PlayerName.Add(Border_PlayerName_3);

	All_Text_PlayerName.Add(Text_PlayerName);
	All_Text_PlayerName.Add(Text_PlayerName_1);
	All_Text_PlayerName.Add(Text_PlayerName_2);
	All_Text_PlayerName.Add(Text_PlayerName_3);

}

void ULobbyMenuUserWidget::OnBttn_MainMenuClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnBttn_MainMenuClick"));
	AShootMenuController* MyController = Cast<AShootMenuController>(GetWorld()->GetFirstPlayerController());
	if (MyController && MyController->HasAuthority()) {
		//OnServer
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer"));
		AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			for (AShootMenuController* Controller : GameMode->All_ControllerPlayers) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnServer :GameMode for"));
				if (Controller != MyController) {
					Controller->DestroyClientSession();
				}
			}
			MyController->DestroyClientSession();
		}
	}
	else {
		//OnClient
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnClient"));
		if (MyController)
		{
			//记得补上删除服务器上的All_ControllerPlayers列表...
			MyController->DestroyClientSession();
			
		}
	}
}

void ULobbyMenuUserWidget::OnBttn_StartGameCilck()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnBttn_StartGameCilck"));
	if (MyGameInstance) {
		AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			MyGameInstance->TriggerTravelToMap(GameMode->All_ControllerPlayers.Num(), TEXT("Level01"));
		}
	}
}

void ULobbyMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AShootMenuController* MyController = Cast<AShootMenuController>(GetWorld()->GetFirstPlayerController());
	if (MyController && MyController->HasAuthority()) {
		//OnServer

	}
	else {
		//OnClient
		Text_StartGameBttn->SetText(FText::FromString(TEXT("Please wait")));
		Bttn_StartGame->SetIsEnabled(false);
	}
}

void ULobbyMenuUserWidget::UpdatePlayerNames(const TArray<FS_PlayerProfile>& PlayerProfiles)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("UpdatePlayerNames%d"), PlayerProfiles.Num()));

	for (int32 i = 0; i < PlayerProfiles.Num();++i) {
		All_Text_PlayerName[i]->SetText(PlayerProfiles[i].PlayerName);
		All_Border_PlayerName[i]->SetBrushColor(FLinearColor(0,0,0,1.f));
	}
}

void ULobbyMenuUserWidget::ClearPlayerNames()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ClearPlayerNames%d"), All_Text_PlayerName.Num()));
	for (int32 i = 0; i < All_Text_PlayerName.Num(); ++i) {
		All_Text_PlayerName[i]->SetText(FText::FromString(TEXT("Wariting for player...")));
		All_Border_PlayerName[i]->SetBrushColor(FLinearColor(0,0,0,0.7f));
	}
}
