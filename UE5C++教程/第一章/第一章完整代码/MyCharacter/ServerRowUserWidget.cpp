// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRowUserWidget.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "TutorialGameInstance.h"
#include "Components/Border.h"
#include "MPUserWidget.h"

void UServerRowUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UpdateSeverRowInfoEvent.AddUObject(this, &UServerRowUserWidget::UpdateServerRow_Info);
	if (Bttn_JoinGame) {
		Bttn_JoinGame->OnClicked.AddDynamic(this, &UServerRowUserWidget::JoinGame);
	}
}

void UServerRowUserWidget::TriggerUpdateServerRowInfo(FOnlineSessionSearchResult& SessionResult)
{
	UpdateSeverRowInfoEvent.Broadcast(SessionResult);
}

void UServerRowUserWidget::UpdateServerRow_Info(FOnlineSessionSearchResult& SessionResult)
{
	Result = &SessionResult;
	if (!Result) {
		UE_LOG(LogTemp, Error, TEXT("UpdateServerRow_Info is failed"));
		return;
	}
	if (Text_ServerName) {
		FString ServerName;
		if (Result->Session.SessionSettings.Get(FName(TEXT("SERVER_NAME")), ServerName)) {
			Text_ServerName->SetText(FText::FromString(ServerName));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("ServerName %s "), *ServerName));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Server Name not found"));
			Text_ServerName->SetText(FText::FromString(TEXT("Server Name not found")));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("Server Name not found")));
		}
	}

	if (Text_NumPlayer) {
		int32 MaxPlayers  = Result->Session.SessionSettings.NumPublicConnections;
		int32 OpenSlots = Result->Session.NumOpenPublicConnections;
		int32 CurrentPlayers = MaxPlayers - OpenSlots;
		FString PlayerCountString = FString::Printf(TEXT("%d/%d"), CurrentPlayers, MaxPlayers);
		FText PlayerCountText = FText::FromString(PlayerCountString);
		Text_NumPlayer->SetText(PlayerCountText);
	}

	if (Text_Ping) {
		Text_Ping->SetText(FText::FromString(FString::Printf(TEXT("%d ms"), Result->PingInMs)));
	}
}

void UServerRowUserWidget::JoinGame()
{
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());

	MyGameInstance->TriggerSessionJoin(*Result);

	if (WB_MpMenu) {
		WB_MpMenu->Border_MenuButton->SetIsEnabled(false);
		WB_MpMenu->Border_ServerList->SetIsEnabled(false);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("JoinGame is failed, WB_MpMenu is nullptr"));
		return;
	}
}
