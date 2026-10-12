// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EnemyHealthWidget.h"
#include "GAS/EnemyAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyHealthWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	if (!InActor) return;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InActor);
	if (!ASI) return;
	UAbilitySystemComponent* AbilitySystemComp = ASI->GetAbilitySystemComponent();
	if (!AbilitySystemComp) return;
	ASC = AbilitySystemComp;

	// Health, MaxHealth가 바뀔 때마다 알려달라고 등록
	ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetHealthAttribute()).AddUObject(this, &UEnemyHealthWidget::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UEnemyHealthWidget::OnMaxHealthChanged);

	// 현재 값으로 처음 한 번 화면 갱신
	bool bFound = false;
	float Value = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetHealthAttribute(), bFound);
	CurrentHealth = bFound ? Value : 0.0f;

	Value = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetMaxHealthAttribute(), bFound);
	MaxHealth = bFound ? Value : 100.0f;

	UpdateHealthUI();
}

void UEnemyHealthWidget::NativeDestruct()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UEnemyHealthWidget::OnHealthChanged(const FOnAttributeChangeData& InData)
{
	CurrentHealth = InData.NewValue;
	UpdateHealthUI();
}

void UEnemyHealthWidget::OnMaxHealthChanged(const FOnAttributeChangeData& InData)
{
	MaxHealth = InData.NewValue;
	UpdateHealthUI();
}

void UEnemyHealthWidget::UpdateHealthUI()
{
	const float Percent = FMath::IsNearlyZero(MaxHealth) ? 0.0f : FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth)));
	}
}
