// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Fireball.generated.h"

class AFireballProjectile;

/**
 * Fireball 어빌리티
 * Mana 소모(코스트)와 쿨다운은 이 어빌리티를 상속한 BP(GA_Fireball)에서 GE로 지정한다.
 */
UCLASS()
class UNREAL10TH_EXAM6_API UGameplayAbility_Fireball : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Fireball();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	// 발사할 투사체 (BP에서 지정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> ProjectileClass;

	// 캐릭터 앞쪽으로 얼마나 떨어진 곳에서 생성할지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	float SpawnForwardOffset = 100.0f;

	// 캐릭터 위쪽으로 얼마나 올릴지 (가슴 높이)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	float SpawnHeightOffset = 40.0f;
};
