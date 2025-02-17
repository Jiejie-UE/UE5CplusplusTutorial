// Fill out your copyright notice in the Description page of Project Settings.


#include "MPUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TutorialGameInstance.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "ServerRowUserWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online.h" 
#include "TimerManager.h"
#include "Online/OnlineSessionNames.h"

void UMPUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Bttn_MainMenu) {
		Bttn_MainMenu->OnClicked.AddDynamic(this, &UMPUserWidget::OnBttn_MainMenuClick);
	}

	if (Bttn_HostGame) {
		Bttn_HostGame->OnClicked.AddDynamic(this, &UMPUserWidget::OnBttn_HostGameClick);
	}

	if (Bttn_SearchGame) {
		Bttn_SearchGame->OnClicked.AddDynamic(this, &UMPUserWidget::OnBttn_SearchGameClick);
	}

	if (Border_ServerList) {
		Border_ServerList->SetVisibility(ESlateVisibility::Hidden);
	}

	//1.4.2
	if (Bttn_ConnectionType) {
		Bttn_ConnectionType->OnClicked.AddDynamic(this, &UMPUserWidget::OnBttn_ConnectionTypeClick);
	}

	MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());

	if (MyGameInstance->SessionInterface.IsValid()) {
		MyGameInstance->SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMPUserWidget::OnFindSessionsComplete);
	}

}

void UMPUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called"));

	SetConnectionType_Text(MyGameInstance->bIsLanConnection);

	ScrollBox_ServerList->ClearChildren();
	ClearAllSeverinfo_WB();
	Text_SearchGameButton->SetText(FText::FromString(TEXT("Searching Game")));

	Border_MenuButton->SetIsEnabled(true);
	Border_ServerList->SetIsEnabled(true);

	Border_ServerList->SetVisibility(ESlateVisibility::Hidden);

	
}

void UMPUserWidget::OnBttn_MainMenuClick()
{
	MyGameInstance->UI_ShowMainMenu();
	RemoveFromParent();
}

void UMPUserWidget::SetConnectionType_Text(bool IsLanConnenction)
{
	if (IsLanConnenction) {
		Text_Connecttype->SetText(FText::FromString(TEXT("LAN")));
	}
	else {
		Text_Connecttype->SetText(FText::FromString(TEXT("Online")));
	}

}

void UMPUserWidget::OnBttn_HostGameClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OnBttn_HostGameClick")));
	MyGameInstance->TriggerSessionCreation();
}

void UMPUserWidget::OnBttn_SearchGameClick()
{
	if (Border_ServerList&& ScrollBox_ServerList) {

		Border_ServerList->SetVisibility(ESlateVisibility::Visible);

		ScrollBox_ServerList->ClearChildren();

		ClearAllSeverinfo_WB();
		Text_SearchGameButton->SetText(FText::FromString(TEXT("Searching...")));
		Border_MenuButton->SetIsEnabled(false);
		//----------------------------------------
		MyGameInstance->SessionSearch = MakeShareable(new FOnlineSessionSearch());
		
		MyGameInstance->SessionSearch->bIsLanQuery = MyGameInstance->bIsLanConnection;
		MyGameInstance->SessionSearch->MaxSearchResults = 10;
		MyGameInstance->SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		
		MyGameInstance->SessionInterface->FindSessions(0, MyGameInstance->SessionSearch.ToSharedRef());
		
	}
}

void UMPUserWidget::ClearAllSeverinfo_WB()
{
	//清空AllServerinfo_WB
	for (UServerRowUserWidget* Widget : AllServerinfo_WB)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();  // 如果它们已添加到视图中，先移除它们
			Widget->ConditionalBeginDestroy();  // 销毁该 Widget（可选，取决于你是否需要清理内存）
		}
	}
	AllServerinfo_WB.Empty();
}

void UMPUserWidget::OnFindSessionsComplete(bool bWassuccessful)
{
	
	Text_SearchGameButton->SetText(FText::FromString(TEXT("Search Game")));
	Border_MenuButton->SetIsEnabled(true);

	if (bWassuccessful) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OnFindSessionsComplete")));//...
		if (!MyGameInstance->SessionSearch.IsValid()) {
			UE_LOG(LogTemp, Error, TEXT("Search is nullptr"));
			return;
		}

		if (!MyGameInstance->SessionSearch->SearchResults.Num()) {
			UE_LOG(LogTemp, Error, TEXT("FindSessions found 0 results"));
		}
		else {
			if (!MyGameInstance->ServerRowUserWidgetClass)
			{
				UE_LOG(LogTemp, Error, TEXT("ServerRowUserWidgetClass is not set"));
				return;
			}
			for (FOnlineSessionSearchResult& Result : MyGameInstance->SessionSearch->SearchResults) {

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("SessionID:%s,OwningUserName:%s"), *Result.GetSessionIdStr(),*Result.Session.OwningUserName));//...
				
				UServerRowUserWidget* ServerRow = CreateWidget<UServerRowUserWidget>(GetWorld(), MyGameInstance->ServerRowUserWidgetClass);
				ServerRow->WB_MpMenu = this;
				ServerRow->TriggerUpdateServerRowInfo(Result);
				AllServerinfo_WB.Add(ServerRow);
				ScrollBox_ServerList->AddChild(ServerRow);
			}
		}

		//1.4.2
		Text_SearchGameButton->SetText(FText::FromString(TEXT("Refresh List")));

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("FindSessions failed"));
	}
}

//1.4.2
void UMPUserWidget::OnBttn_ConnectionTypeClick()
{
	MyGameInstance->ChangeConnectionType();
	SetConnectionType_Text(MyGameInstance->bIsLanConnection);
}

