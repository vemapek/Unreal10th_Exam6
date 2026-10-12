// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	// 초기값 설정
	InitHealth(100.0f);
	InitMaxHealth(100.0f);

	InitMana(100.0f);
	InitMaxMana(100.0f);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Health는 0 ~ MaxHealth 사이로 유지
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		// Mana는 0 ~ MaxMana 사이로 유지
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxHealthAttribute() || Attribute == GetMaxManaAttribute())
	{
		// 최대값은 음수가 될 수 없다.
		NewValue = FMath::Max(0.0f, NewValue);
	}
}
