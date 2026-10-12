// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EnemyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "ExamEnemyCharacter.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitDamage(0.0f);
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 이펙트가 Damage에 값을 넣었다.
		const float LocalDamage = GetDamage();
		SetDamage(0.0f);	// [중요] 메타 어트리뷰트는 사용했으면 비운다.

		if (LocalDamage > 0.0f)
		{
			// 받은 데미지만큼 Health를 깎는다.
			const float NewHealth = FMath::Clamp(GetHealth() - LocalDamage, 0.0f, GetMaxHealth());
			SetHealth(NewHealth);

			if (NewHealth <= 0.0f)
			{
				// 체력이 0이 되었으면 죽음 처리
				if (AExamEnemyCharacter* Enemy = Cast<AExamEnemyCharacter>(Data.Target.GetAvatarActor()))
				{
					Enemy->OnDeath();
				}
			}
		}
	}
}
