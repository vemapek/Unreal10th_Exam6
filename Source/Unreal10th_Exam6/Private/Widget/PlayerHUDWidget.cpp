// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerHUDWidget.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	if (!InActor) return;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InActor);
	if (!ASI) return;
	UAbilitySystemComponent* AbilitySystemComp = ASI->GetAbilitySystemComponent();
	if (!AbilitySystemComp) return;
	ASC = AbilitySystemComp;

	// 값이 바뀔 때마다 알려달라고 등록
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UPlayerHUDWidget::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UPlayerHUDWidget::OnMaxHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).AddUObject(this, &UPlayerHUDWidget::OnManaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UPlayerHUDWidget::OnMaxManaChanged);

	// 현재 값으로 처음 한 번 화면 갱신
	bool bFound = false;
	float Value = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetHealthAttribute(), bFound);
	CurrentHealth = bFound ? Value : 0.0f;

	Value = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetMaxHealthAttribute(), bFound);
	MaxHealth = bFound ? Value : 100.0f;

	Value = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetManaAttribute(), bFound);
	CurrentMana = bFound ? Value : 0.0f;

	Value = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetMaxManaAttribute(), bFound);
	MaxMana = bFound ? Value : 100.0f;

	UpdateHealthUI();
	UpdateManaUI();
}

void UPlayerHUDWidget::NativeDestruct()
{
	// 등록했던 콜백 해제
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute()).RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UPlayerHUDWidget::OnHealthChanged(const FOnAttributeChangeData& InData)
{
	CurrentHealth = InData.NewValue;
	UpdateHealthUI();
}

void UPlayerHUDWidget::OnMaxHealthChanged(const FOnAttributeChangeData& InData)
{
	MaxHealth = InData.NewValue;
	UpdateHealthUI();
}

void UPlayerHUDWidget::OnManaChanged(const FOnAttributeChangeData& InData)
{
	CurrentMana = InData.NewValue;
	UpdateManaUI();
}

void UPlayerHUDWidget::OnMaxManaChanged(const FOnAttributeChangeData& InData)
{
	MaxMana = InData.NewValue;
	UpdateManaUI();
}

void UPlayerHUDWidget::UpdateHealthUI()
{
	const float Percent = FMath::IsNearlyZero(MaxHealth) ? 0.0f : FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("HP %.0f / %.0f"), CurrentHealth, MaxHealth)));
	}
}

void UPlayerHUDWidget::UpdateManaUI()
{
	const float Percent = FMath::IsNearlyZero(MaxMana) ? 0.0f : FMath::Clamp(CurrentMana / MaxMana, 0.0f, 1.0f);
	if (ManaProgressBar)
	{
		ManaProgressBar->SetPercent(Percent);
	}
	if (ManaText)
	{
		ManaText->SetText(FText::FromString(FString::Printf(TEXT("MP %.0f / %.0f"), CurrentMana, MaxMana)));
	}
}
