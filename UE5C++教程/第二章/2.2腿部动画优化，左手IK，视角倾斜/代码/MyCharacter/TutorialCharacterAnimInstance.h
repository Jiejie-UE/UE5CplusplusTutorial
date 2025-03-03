// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TutorialCharacterAnimInstance.generated.h"

class ATutorialCharacter;
/**
 * 
 */
UCLASS()
class UETUTORIAL_API UTutorialCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATutorialCharacter* MyCharacter;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AimRotation;

public:
	UTutorialCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION()
	void PlayTurnMonatage(float ParmsYawOffset);

	UPROPERTY()
	bool IsPlayingTurnMontage;

	UFUNCTION()
	void Reset_TurnMontage(float Diration);

	UPROPERTY()
	FTimerHandle Reset_TurnMontageTimerHandle;

	UFUNCTION()
	void Reset_TurnMontageDelay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnAnimYaw;

	UFUNCTION()
	void SetLeanangle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CharacterRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawDeltasinceLastUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawDeltaSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddtiveLeanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFirstUpdate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha_MoveSpeed;
};
