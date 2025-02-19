// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "TutorialCharacter.h"
#include "KismetAnimationLibrary.h"



UTutorialCharacterAnimInstance::UTutorialCharacterAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
	bAiming = false;
	GunPitch = 0.f;
}

void UTutorialCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MyCharacter) {
		Velocity = MyCharacter->GetVelocity();

		Speed = Velocity.Size();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, MyCharacter->GetBaseAimRotation());

		bAiming = MyCharacter->bAiming;
		GunPitch = MyCharacter->GetBaseAimRotation().Pitch;
		if (GunPitch > 90) {
			GunPitch -= 360;
		}
		YawOffset = MyCharacter->YawOffset;
		if (Speed == 0.0f && IsPlayingTurnMontage == false) {
			PlayTurnMonatage(MyCharacter->TurnAnimYaw);
		}
	}
}

void UTutorialCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MyCharacter = Cast<ATutorialCharacter>(TryGetPawnOwner());
	if (!MyCharacter) {
		UE_LOG(LogTemp, Error, TEXT("TryGetPawnOwner is nullptr"));
	}
}

void UTutorialCharacterAnimInstance::PlayTurnMonatage(float ParmsYawOffset)
{
	UAnimMontage* LocalTurnMontageToPlay = nullptr;
	if (abs(ParmsYawOffset) < 60.0f) {
		return;
	}
	else {
		if (abs(ParmsYawOffset) >= 135.0f) {
			LocalTurnMontageToPlay = LoadObject<UAnimMontage>(nullptr,TEXT("AnimMontage'/Game/Characters/Heroes/Mannequin/Animations/Locomotion/Pistol/MF_Pistol_TurnRight_180_Montage.MF_Pistol_TurnRight_180_Montage'"));
			if (LocalTurnMontageToPlay) {
				IsPlayingTurnMontage = true;
				float Diration = Montage_Play(LocalTurnMontageToPlay, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
				Reset_TurnMontage(Diration);
			}
		}
		else if (ParmsYawOffset >=60.0f) {
			LocalTurnMontageToPlay = LoadObject<UAnimMontage>(nullptr, TEXT("AnimMontage'/Game/Characters/Heroes/Mannequin/Animations/Locomotion/Pistol/MF_Pistol_TurnRight_90_Montage.MF_Pistol_TurnRight_90_Montage'"));
			if (LocalTurnMontageToPlay) {
				IsPlayingTurnMontage = true;
				float Diration = Montage_Play(LocalTurnMontageToPlay, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
				Reset_TurnMontage(Diration);
			}
		}
		else if (ParmsYawOffset <=-60.0f) {
			LocalTurnMontageToPlay = LoadObject<UAnimMontage>(nullptr, TEXT("AnimMontage'/Game/Characters/Heroes/Mannequin/Animations/Locomotion/Pistol/MF_Pistol_TurnLeft_90_Montage.MF_Pistol_TurnLeft_90_Montage'"));
			if (LocalTurnMontageToPlay) {
				IsPlayingTurnMontage = true;
				float Diration = Montage_Play(LocalTurnMontageToPlay, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
				Reset_TurnMontage(Diration);
			}
		}
	} 
	
}

void UTutorialCharacterAnimInstance::Reset_TurnMontage(float Diration)
{
	GetWorld()->GetTimerManager().SetTimer(Reset_TurnMontageTimerHandle, this, &UTutorialCharacterAnimInstance::Reset_TurnMontageDelay, Diration, false);
}

void UTutorialCharacterAnimInstance::Reset_TurnMontageDelay()
{
	IsPlayingTurnMontage = false;
}


