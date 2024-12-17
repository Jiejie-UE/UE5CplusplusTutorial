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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunPitch;

	ATutorialCharacter* MyCharacter;

	FVector Velocity;

public:
	UTutorialCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeInitializeAnimation() override;

};
