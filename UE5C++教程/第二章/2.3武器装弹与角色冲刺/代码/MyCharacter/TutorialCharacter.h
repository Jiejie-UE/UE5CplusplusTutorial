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
class UInGameHUDUserWidget;
class UAnimMontage;
class UAnimationAsset;

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
	//1.6
	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(Client, Reliable, Category = "Lobby")
	void DisableAllInput(APlayerController* PlayerController);
	void DisableAllInput_Implementation(APlayerController* PlayerController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseGameAction;

	void OpenPauseGame();

	UPROPERTY()
	FTimerHandle BeginPlayTimerHandle;

	UFUNCTION()
	void BeginPlayDelay1();

	UPROPERTY()
	UInGameHUDUserWidget* InGameHUD;

	//2.1.2
	//UPROPERTY()
	//float New_YawOffset;
	//UPROPERTY(Replicated)
	//float YawOffset;

	//UFUNCTION(Server, Reliable, Category = "Lobby")
	//void Server_SetYawOffset(float ParamYawOffset, float ParamTurnAnimYaw);
	//void Server_SetYawOffset_Implementation(float ParamYawOffset, float ParamTurnAnimYaw);

	//UPROPERTY()
	//float New_TurnAnimYaw;
	//UPROPERTY(Replicated)
	//float TurnAnimYaw;

	UFUNCTION(Server, Reliable, Category = "Lobby")
	void Server_SetUsedesiredRot(bool UseControllerDesiredrotation);
	void Server_SetUsedesiredRot_Implementation(bool UseControllerDesiredrotation);

	UPROPERTY()
	bool bIsMoving;

	UFUNCTION()
	void UpdateMoveState(bool ParamIsMoving);

	UPROPERTY(Replicated)
	FRotator AimRotation;

	UPROPERTY()
	FRotator New_AimRotation;

	UFUNCTION(Server, Reliable, Category = "Lobby")
	void Server_SetAimRotation(FRotator ParamAimRotation);
	void Server_SetAimRotation_Implementation(FRotator ParamAimRotation);

	//2.3
	void Play_FireMontage();

	UFUNCTION(Server, Reliable, Category = "Lobby")
	void Server_FireMontage();
	void Server_FireMontage_Implementation();

	UFUNCTION(NetMulticast, Reliable, Category = "Lobby")
	void Multicast_FireMontage();
	void Multicast_FireMontage_Implementation();

	//ÐèÌîÐ´£¬ÔÚ±à¼­Æ÷ÖÐ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* MM_Pistol_Fire_Montage;
	//ÐèÌîÐ´£¬ÔÚ±à¼­Æ÷ÖÐ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* Weap_Pistol_Fire;

	UPROPERTY()
	bool bIsReloading = false;

	void GunReloading();
	//ÐèÌîÐ´£¬ÔÚ±à¼­Æ÷ÖÐ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* GunReloadAction;

	void Play_ReloadMontage();

	UFUNCTION(Server, Reliable, Category = "Lobby")
	void Server_ReloadMontage();
	void Server_ReloadMontage_Implementation();

	UFUNCTION(NetMulticast, Reliable, Category = "Lobby")
	void Multicast_ReloadMontage();
	void Multicast_ReloadMontage_Implementation();

	//ÐèÌîÐ´£¬ÔÚ±à¼­Æ÷ÖÐ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* MM_Pistol_Reload_Montage;
	//ÐèÌîÐ´£¬ÔÚ±à¼­Æ÷ÖÐ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* Weap_Pistol_Reload;

	void Reset_IsReloading(float Diration);

	UPROPERTY()
	FTimerHandle Reset_IsReloadingTimerHandle;

	void Set_IsReloading_false();

};
