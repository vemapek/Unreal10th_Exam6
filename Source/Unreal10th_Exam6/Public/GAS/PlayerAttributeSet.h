// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 플레이어용 AttributeSet
 * Health, Mana 와 각각의 최대값을 가진다.
 */
UCLASS()
class UNREAL10TH_EXAM6_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	// CurrentValue 변경 전에 실행되는 함수
	// 값의 Clamping 용도로 사용
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// ------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Player Stat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Player Stat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Player Stat")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Player Stat")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxMana);
};
