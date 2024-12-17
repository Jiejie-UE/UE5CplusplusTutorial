// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TutorialCharacter.generated.h"
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class USkeletalMeshComponent;
class UNiagaraSystem;
class USoundBase;
struct FInputActionValue;

UCLASS()
class UETUTORIAL_API ATutorialCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATutorialCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* GunAimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* GunShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunInput, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* GunMappingContext;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunInput, meta = (AllowPrivateAccess = "true"), Replicated)
	bool bAiming;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void GuninAim(const FInputActionValue& Value);
	void GunoutAim(const FInputActionValue& Value);
	void GunShoot(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

public:
	void BindGunInputMappingContext();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool aiming);
	bool Server_SetAiming_Validate(bool aiming);
	void Server_SetAiming_Implementation(bool aiming);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* GunShootNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* GunShotSound;
};
