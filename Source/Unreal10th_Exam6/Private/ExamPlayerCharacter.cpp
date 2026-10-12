// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamPlayerCharacter.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

AExamPlayerCharacter::AExamPlayerCharacter()
{
	// ASC와 AttributeSet 생성
	// AttributeSet을 같은 액터의 DefaultSubobject로 만들면 ASC가 자동으로 찾아서 등록한다.
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));

	// 카메라
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 카메라가 도는 것과 캐릭터 회전은 별개로 한다.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

UAbilitySystemComponent* AExamPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AExamPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		// (소유자, 실제 몸) 둘 다 이 캐릭터
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AExamPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			Enhanced->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExamPlayerCharacter::Move);
		}
		if (LookAction)
		{
			Enhanced->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExamPlayerCharacter::Look);
		}
		if (JumpAction)
		{
			Enhanced->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			Enhanced->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
	}
}

void AExamPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D Input = Value.Get<FVector2D>();

	// 카메라가 보는 방향(Yaw) 기준으로 앞/오른쪽 방향을 구한다.
	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Input.Y);
	AddMovementInput(Right, Input.X);
}

void AExamPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}
