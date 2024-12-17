// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "TutorialCharacter.h"


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
		Direction = CalculateDirection(Velocity, MyCharacter->GetBaseAimRotation());

		bAiming = MyCharacter->bAiming;
		GunPitch = MyCharacter->GetBaseAimRotation().Pitch;
		if (GunPitch > 90) {
			GunPitch -= 360;
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


