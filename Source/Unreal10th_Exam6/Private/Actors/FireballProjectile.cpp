// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 충돌용 구체 (루트)
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(25.0f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SetRootComponent(Collision);

	// 눈에 보이는 구체 (엔진 기본 도형 사용)
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}

	// 앞으로 날아가게 하는 컴포넌트
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1500.0f;
	Movement->MaxSpeed = 1500.0f;
	Movement->ProjectileGravityScale = 0.0f;	// 중력 영향 없음

	InitialLifeSpan = 5.0f;	// 아무것도 안 맞아도 5초 뒤 사라짐
}

void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 쏜 사람(플레이어)과는 부딪히지 않게 한다.
	if (GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	}

	Collision->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnProjectileHit);
}

void AFireballProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 맞은 대상에게 데미지를 주고 사라진다. (나중에 Burn도 여기서 처리)
	ApplyDamage(OtherActor, Hit);
	Destroy();
}

void AFireballProjectile::ApplyDamage(AActor* Target, const FHitResult& Hit)
{
	if (!Target || !DamageEffectClass) return;

	// 맞은 대상이 ASC를 가지고 있어야 한다. (벽 같은 것에 맞았으면 여기서 끝)
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return;

	// 쏜 사람의 ASC가 이펙트를 적용한다. (없으면 대상의 ASC로 대신)
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	UAbilitySystemComponent* ApplyingASC = SourceASC ? SourceASC : TargetASC;

	// 이펙트 Context: "누가, 무엇으로, 어디를 맞췄는지" 정보
	FGameplayEffectContextHandle Context = ApplyingASC->MakeEffectContext();
	Context.AddSourceObject(this);
	Context.AddHitResult(Hit);

	// Spec = 이펙트 클래스 + Context. 이걸 대상에게 적용한다.
	FGameplayEffectSpecHandle Spec = ApplyingASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);
	if (Spec.IsValid())
	{
		ApplyingASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	}
}
