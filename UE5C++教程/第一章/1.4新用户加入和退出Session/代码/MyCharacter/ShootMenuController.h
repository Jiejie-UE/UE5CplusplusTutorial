// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../MyCharacter/S_PlayerProfile.h"
#include "ShootMenuController.generated.h"


class UTutorialGameInstance;
class ULobbyMenuUserWidget;

/**
 * 
 */
UCLASS()
class UETUTORIAL_API AShootMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	UTutorialGameInstance* MyGameInstance;
	
public:

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void DestroyClientSession();
	void DestroyClientSession_Implementation();

	
	void CreateLobbyMenu();
	
	UPROPERTY()
	ULobbyMenuUserWidget* LobbyMenu;

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void ClientCreateLobbyMenu();
	void ClientCreateLobbyMenu_Implementation();

	UFUNCTION()
	void LoadSavedProfile();

	UPROPERTY()
	FS_PlayerProfile PlayerProfileinfo;

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void Init_Setup();
	void Init_Setup_Implementation();

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void UpdatePlayerNames(const TArray<FS_PlayerProfile>& PlayerProfiles);
	void UpdatePlayerNames_Implementation(const TArray<FS_PlayerProfile>& PlayerProfiles);

	UFUNCTION(Server, Reliable)
	void Server_UpdateLobby(FS_PlayerProfile PlayerProfile);
	void Server_UpdateLobby_Implementation(FS_PlayerProfile PlayerProfile);

};
