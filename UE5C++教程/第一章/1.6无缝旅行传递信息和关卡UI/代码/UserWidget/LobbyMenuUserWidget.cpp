// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenuUserWidget.h"
#include "Components/Button.h"
#include "../MyCharacter/ShootMenuController.h"
#include "../MyCharacter/ShootMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../MyCharacter/TutorialGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"
#include "../LevelInfo/S_LevelInfo.h"
#include "../UserWidget/LevelRowInfoUserWidget.h"
#include "Components/Image.h"
#include "../MyCharacter/ServerRowUserWidget.h"


void ULobbyMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Bttn_MainMenu) {
		Bttn_MainMenu->OnClicked.AddDynamic(this, &ULobbyMenuUserWidget::OnBttn_MainMenuClick);
	}
	if (Bttn_StartGame) {
		Bttn_StartGame->OnClicked.AddDynamic(this, &ULobbyMenuUserWidget::OnBttn_StartGameCilck);
	}
	//1.5.1
	if (CheckBox_SeamlessTravel) {
		CheckBox_SeamlessTravel->OnCheckStateChanged.AddDynamic(this, &ULobbyMenuUserWidget::OnCheckBox_SeamlessTravelChanged);
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
			for (int32 i = 0; i < GameMode->All_ControllerPlayers.Num();++i) {
				GameMode->All_ControllerPlayers[i]->ClientCreateLoadScreen();
			}
			//1.5.1
			MyGameInstance->TriggerTravelToMap(GameMode->All_ControllerPlayers.Num(), SelectedLevel_String);
		}
	}
}

void ULobbyMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AShootMenuController* MyController = Cast<AShootMenuController>(GetWorld()->GetFirstPlayerController());
	if (MyController && MyController->HasAuthority()) {
		//OnServer
		//1.5.1
		AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			CheckBox_SeamlessTravel->SetIsChecked(GameMode->bUseSeamlessTravel);
		}

		All_LevelInfoRows.Empty();
		ScrollBox_AllLevelName->ClearChildren();
		if (!LevelInfo_Table) {
			LevelInfo_Table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/MyComponent/LevelInfo/LevelInfo_Table.LevelInfo_Table'"));
		}
		if (LevelInfo_Table) {
			FString ContextString = TEXT("GetAllRows Context");
			TArray<FS_LevelInfo*> AllRows;
			LevelInfo_Table->GetAllRows<FS_LevelInfo>(ContextString, AllRows);
			for (FS_LevelInfo* Row : AllRows)
			{
				if (Row)
				{
					ULevelRowInfoUserWidget* LevelRowInfo = CreateWidget<ULevelRowInfoUserWidget>(GetWorld(), MyGameInstance->LevelRowInfoWidgetClass);
					if (LevelRowInfo) {
						LevelRowInfo->Map_displayName = Row->DisplayName;
						LevelRowInfo->TableRowName = FName(*Row->DisplayName.ToString());
						LevelRowInfo->WB_LobbyMenu = this;
						All_LevelInfoRows.Add(LevelRowInfo);
						ScrollBox_AllLevelName->AddChild(LevelRowInfo);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("LevelRowInfo")));
					}
				}
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ScrollBox_AllLevelName->AddChild(LevelRowInfo)")));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ScrollBox_AllLevelName : %d"), ScrollBox_AllLevelName->GetChildrenCount()));
			if (AllRows.Num() > 0) {
				Server_SelectLevel(FName(*AllRows[0]->DisplayName.ToString()));
			}
		}
	}
	else {
		//OnClient
		Text_StartGameBttn->SetText(FText::FromString(TEXT("Please wait")));
		Bttn_StartGame->SetIsEnabled(false);
		//1.5.1
		Border_SeamlessTravel->SetVisibility(ESlateVisibility::Hidden);

		ScrollBox_AllLevelName->RemoveFromParent();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OnClient ScrollBox_AllLevelName->RemoveFromParent()")));
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

void ULobbyMenuUserWidget::OnCheckBox_SeamlessTravelChanged(bool IsClick)
{
	AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) {
		GameMode->bUseSeamlessTravel = IsClick;
	}

}

void ULobbyMenuUserWidget::UpdatelevelSelection(FName LevelRowName)
{
	FString ContextString = TEXT("FindRow Context");
	if (!LevelInfo_Table) {
		LevelInfo_Table = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/MyComponent/LevelInfo/LevelInfo_Table.LevelInfo_Table'"));
	}
	FS_LevelInfo* Row = LevelInfo_Table->FindRow<FS_LevelInfo>(LevelRowName, ContextString, true);
	Text_SelectedMapName->SetText(Row->DisplayName);
	Image_PreviewImage->SetBrushFromTexture(Row->PreviewImage);
	SelectedLevel_String = Row->TravelString;
}

void ULobbyMenuUserWidget::Server_SelectLevel(FName LevelRowName)
{
	AShootMenuGameMode* GameMode = Cast<AShootMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) {
		GameMode->SelectedLevelName = LevelRowName;
		GameMode->UpdateLobby(false,true);
	}
}
