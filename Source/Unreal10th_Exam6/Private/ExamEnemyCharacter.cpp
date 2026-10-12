// Fill out your copyright notice in the Description page of Project Settings.


#include "ExamEnemyCharacter.h"
#include "GAS/EnemyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Widget/EnemyHealthWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AExamEnemyCharacter::AExamEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	// 머리 위 위젯
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidgetComponent->SetupAttachment(RootComponent);
	OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);	// 화면 기준이라 항상 카메라를 바라본다.
	OverHeadWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
	OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AExamEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AExamEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 적은 컨트롤러 없이 월드에 배치되므로 BeginPlay에서 ASC를 초기화한다.
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// 위젯에 이 적의 ASC를 알려준다.
	if (OverHeadWidgetComponent)
	{
		if (UEnemyHealthWidget* HealthWidget = Cast<UEnemyHealthWidget>(OverHeadWidgetComponent->GetUserWidgetObject()))
		{
			HealthWidget->InitializeWithAbilitySystem(this);
		}
	}
}

void AExamEnemyCharacter::OnDeath()
{
	if (bIsDead) return;
	bIsDead = true;

	UE_LOG(LogTemp, Log, TEXT("[%s] 사망"), *GetName());

	// 더 이상 맞지도 움직이지도 않게 하고, 잠시 뒤 제거
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverHeadWidgetComponent->SetVisibility(false);	// 체력 바 숨기기
	SetLifeSpan(2.0f);
}
