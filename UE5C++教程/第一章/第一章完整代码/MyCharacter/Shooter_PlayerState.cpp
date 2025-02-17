// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter_PlayerState.h"
#include "Net/UnrealNetwork.h"

//只有无缝旅行才有效
void AShooter_PlayerState::CopyProperties(APlayerState* PlayerState) {
	Super::CopyProperties(PlayerState);

    if (AShooter_PlayerState* ShooterPS = Cast<AShooter_PlayerState>(PlayerState))
    {
        ShooterPS->PlayerProfile = this->PlayerProfile;
    }
}

void AShooter_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AShooter_PlayerState, PlayerProfile);
}
