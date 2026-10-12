// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnemyAttributeSet.generated.h"

/**
 * 적 캐릭터용 AttributeSet
 * Health, MaxHealth 와 데미지 계산용 메타 어트리뷰트(Damage)를 가진다.
 */
UCLASS()
class UNREAL10TH_EXAM6_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEnemyAttributeSet();

	// CurrentValue 변경 전에 실행 (Clamping 용도)
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// 이펙트가 적용된 후에 실행 (Damage -> Health 변환, 죽음 처리)
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// ------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Stat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Stat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, MaxHealth);

	// ------------------------------------------------------------------
	// 메타 어트리뷰트 : 값을 저장하는 용도가 아니라, 데미지를 전달하는 용도
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, Damage);
};
