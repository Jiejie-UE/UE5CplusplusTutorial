// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h" 
#include "TutorialGameInstance.h"
#include "ShooterPlayerController.h"
#include "../UserWidget/InGameHUDUserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATutorialCharacter::ATutorialCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->bDoCollisionTest = true;                // ������ײ���
	//SpringArm->ProbeSize = 12.0f;                      // ���ü�������С
	//SpringArm->CameraLagSpeed = 3.0f;                  // ��������ӳ��ٶ�
	//SpringArm->bEnableCameraLag = true;                // �������ƽ���ӳ�


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	//CameraComponent->FieldOfView = 90.0f;                       // ��������ӳ���
	Camera->bUsePawnControlRotation = false;

	//2.1.2
	bUseControllerRotationYaw = false;	//����������ƽ�ɫ�Ƿ�����������Yaw��ת

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	//2.1.
	GetCharacterMovement()->RotationRate.Yaw = 360.0f;//���ֵ����Ӱ��Ӿ�ֹ���ƶ��Ĺ����ٶ�
	
}

// Called when the game starts or when spawned
void ATutorialCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	bAiming = false;
	bIsMoving = false;

	GetWorld()->GetTimerManager().SetTimer(BeginPlayTimerHandle, this, &ATutorialCharacter::BeginPlayDelay1, 2.0f, false);
}

// Called every frame
void ATutorialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = GetVelocity();
	float XYLength = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	bool ParamIsMoving = XYLength > 0;
	UpdateMoveState(ParamIsMoving);
	
	New_AimRotation = GetBaseAimRotation();

	if (HasAuthority()) {
		AimRotation = New_AimRotation;
	}
	else {
		Server_SetAimRotation(New_AimRotation);
		AimRotation = New_AimRotation;
	}

}

// Called to bind functionality to input
void ATutorialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATutorialCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATutorialCharacter::Look);

		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ATutorialCharacter::OpenPauseGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	BindGunInputMappingContext();

}


void ATutorialCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATutorialCharacter, bAiming);
	//2.1
	//DOREPLIFETIME(ATutorialCharacter, YawOffset);
	//DOREPLIFETIME(ATutorialCharacter, TurnAnimYaw);
	//2.2
	DOREPLIFETIME(ATutorialCharacter, AimRotation);
}


void ATutorialCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATutorialCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATutorialCharacter::GuninAim(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("GuninAim is success"));

	if (HasAuthority()) {
		//�ڷ������
		bAiming = true;
	}
	else {
		//�ڿͻ�����Ҳ�����ⲽ�����������ӳ���
		bAiming = true;

		//�ڿͻ�������Ҫת��
		Server_SetAiming(true);
	}
	
}

void ATutorialCharacter::GunoutAim(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("GunoutAim is success"));
	if (HasAuthority()) {
		//�ڷ������
		bAiming = false;
	}
	else {
		//�ڿͻ�����Ҳ�����ⲽ�����������ӳ���
		bAiming = false;

		//�ڿͻ�������Ҫת��
		Server_SetAiming(false);
	}
	
}

void ATutorialCharacter::GunShoot(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("GunShoot is success"));

	
	if (GunShootNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(GunShootNiagaraSystem, WeaponMesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}
	if (GunShotSound) {
		UGameplayStatics::PlaySound2D(this, GunShotSound);
	}
	FTransform Temp = WeaponMesh->GetSocketTransform(TEXT("Muzzle"));
	FVector Start = Temp.GetLocation();
	FVector End = Temp.GetRotation().GetForwardVector() * 100000 + Start;
	//GetWorld()->LineTraceTestByChannel(Start, End, ECC_Visibility);
	// ���Կ��ӻ�����
	DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,5.0f,0,1.0f);
}

void ATutorialCharacter::BindGunInputMappingContext()
{
	if (!GunMappingContext) {
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(GunMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			EnhancedInputComponent->BindAction(GunAimAction, ETriggerEvent::Started, this, &ATutorialCharacter::GuninAim);
			EnhancedInputComponent->BindAction(GunAimAction, ETriggerEvent::Completed, this, &ATutorialCharacter::GunoutAim);
			
			EnhancedInputComponent->BindAction(GunShootAction, ETriggerEvent::Triggered, this, &ATutorialCharacter::GunShoot);

		}
		
	}
}

bool ATutorialCharacter::Server_SetAiming_Validate(bool aiming)
{
	return true;
}

void ATutorialCharacter::Server_SetAiming_Implementation(bool aiming)
{
	bAiming = aiming;
}

//OnServer���������ֻ���ڷ��������
void ATutorialCharacter::PossessedBy(AController* NewController)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("PossessedBy")));
	Super::PossessedBy(NewController);
	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (PlayerController) {
		DisableAllInput(PlayerController);
	}
}

void ATutorialCharacter::DisableAllInput_Implementation(APlayerController* PlayerController)
{
	//����ط����Գ�������ǿ������Ǹ���ֹ������ϸ����
	DisableInput(PlayerController);
}

void ATutorialCharacter::OpenPauseGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString::Printf(TEXT("OpenPauseGame")));
	UTutorialGameInstance* MyGameInstance = Cast<UTutorialGameInstance>(GetGameInstance());
	if (MyGameInstance && InGameHUD) {
		if (MyGameInstance->TogglePauseGame()) {
			InGameHUD->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			InGameHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ATutorialCharacter::BeginPlayDelay1()
{
	if (IsLocallyControlled()) {
		SetActorTickEnabled(true);
		AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
		if (PlayerController) {
			InGameHUD = PlayerController->InGameHUD;
		}
	}
	else {
		//2.1
		SetActorTickEnabled(false);
	}
}

//void ATutorialCharacter::Server_SetYawOffset_Implementation(float ParamYawOffset, float ParamTurnAnimYaw)
//{
//	YawOffset = ParamYawOffset;
//	TurnAnimYaw = ParamTurnAnimYaw;
//}

void ATutorialCharacter::Server_SetUsedesiredRot_Implementation(bool UseControllerDesiredrotation)
{
	GetCharacterMovement()->bUseControllerDesiredRotation = UseControllerDesiredrotation;
}

void ATutorialCharacter::UpdateMoveState(bool ParamIsMoving)
{
	if (ParamIsMoving != bIsMoving) {
		bIsMoving = ParamIsMoving;
		if (HasAuthority()) {
			GetCharacterMovement()->bUseControllerDesiredRotation = bIsMoving;
		}
		else {
			Server_SetUsedesiredRot(bIsMoving);
			GetCharacterMovement()->bUseControllerDesiredRotation = bIsMoving;
		}
	}
	else {
		bIsMoving = ParamIsMoving;
	}
}

void ATutorialCharacter::Server_SetAimRotation_Implementation(FRotator ParamAimRotation)
{
	AimRotation = ParamAimRotation;
}








