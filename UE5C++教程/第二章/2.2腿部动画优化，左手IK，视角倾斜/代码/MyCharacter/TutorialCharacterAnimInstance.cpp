// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "TutorialCharacter.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

UTutorialCharacterAnimInstance::UTutorialCharacterAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
	//bAiming = false;
	GunPitch = 0.f;
	bIsFirstUpdate = true;
}

void UTutorialCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MyCharacter) {
		SetLeanangle();
		//2.2
		AimRotation = MyCharacter->AimRotation;

		Velocity = MyCharacter->GetVelocity();

		Speed = Velocity.Size();

		GunPitch = FRotator::NormalizeAxis(AimRotation.Pitch);

		FVector VelocityZ = Velocity; VelocityZ.X = 0; VelocityZ.Y = 0;
		FRotator AimRotationYaw = AimRotation; AimRotationYaw.Roll = 0; AimRotationYaw.Pitch = 0;
		Direction = UKismetAnimationLibrary::CalculateDirection(VelocityZ, AimRotationYaw);
		
		TurnAnimYaw = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, MyCharacter->GetActorRotation()).Yaw;

		YawOffset = FMath::FInterpTo(YawOffset, FMath::Clamp(TurnAnimYaw,-60,60), DeltaSeconds,15.0f);

		//2.2
		Alpha_MoveSpeed = UKismetMathLibrary::MapRangeClamped(Speed, 0.0f, MaxWalkSpeed, 0.0f, 1.0f);

		if (Speed == 0.0f && IsPlayingTurnMontage == false) {
			PlayTurnMonatage(TurnAnimYaw);
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
	else {
		MaxWalkSpeed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
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

void UTutorialCharacterAnimInstance::SetLeanangle()
{
	YawDeltasinceLastUpdate = MyCharacter->GetActorRotation().Yaw - CharacterRotation.Yaw;
	if (GetDeltaSeconds() != 0.0f) {
		YawDeltaSpeed = YawDeltasinceLastUpdate / GetDeltaSeconds();
	}
	else {
		YawDeltaSpeed = 0;
	}
	CharacterRotation = MyCharacter->GetActorRotation();
	AddtiveLeanAngle = YawDeltaSpeed * (MyCharacter->bAiming ? 0.05f : 0.1f); 
	if (bIsFirstUpdate) {
		YawDeltasinceLastUpdate = 0;
		YawDeltaSpeed = 0;
		bIsFirstUpdate = false;
	}
}


