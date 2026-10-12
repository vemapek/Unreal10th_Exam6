// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Fireball.h"
#include "Actors/FireballProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 코스트(Mana)와 쿨다운 검사 후, 가능하면 적용
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 마지막 true는 정상적인 종료가 아니라는 표시
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !ProjectileClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 카메라가 보는 방향(Yaw)으로 캐릭터를 돌려서, 캐릭터 전방으로 발사되게 한다.
	if (const AController* Controller = Character->GetController())
	{
		Character->SetActorRotation(FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f));
	}

	const FVector SpawnLocation = Character->GetActorLocation()
		+ Character->GetActorForwardVector() * SpawnForwardOffset
		+ FVector(0.0f, 0.0f, SpawnHeightOffset);
	const FRotator SpawnRotation = Character->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Character->GetWorld()->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);	// 성공적으로 끝났다.
}
