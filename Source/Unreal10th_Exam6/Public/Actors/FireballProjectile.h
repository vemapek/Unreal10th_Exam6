// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UGameplayEffect;

/**
 * Fireball 어빌리티가 생성하는 투사체
 * 부딪힌 대상에게 데미지 GE를 적용하고 사라진다. (Burn은 나중에 추가)
 */
UCLASS()
class UNREAL10TH_EXAM6_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 맞은 대상에게 데미지 이펙트를 적용한다.
	void ApplyDamage(AActor* Target, const FHitResult& Hit);

protected:
	// 명중 시 적용할 데미지 이펙트 (BP에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball")
	TObjectPtr<UProjectileMovementComponent> Movement;
};
