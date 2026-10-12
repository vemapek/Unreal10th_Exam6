// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ExamEnemyCharacter.generated.h"

class UAbilitySystemComponent;
class UEnemyAttributeSet;
class UWidgetComponent;

/**
 * 적 캐릭터
 * ASC(AbilitySystemComponent)와 EnemyAttributeSet을 가지고 있다.
 */
UCLASS()
class UNREAL10TH_EXAM6_API AExamEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AExamEnemyCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UEnemyAttributeSet* GetEnemyAttributeSet() const { return EnemyAttributeSet; }

	// 체력이 0이 되었을 때 AttributeSet이 호출한다.
	void OnDeath();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UEnemyAttributeSet> EnemyAttributeSet;

	// 머리 위에 Health를 보여주는 위젯 컴포넌트 (위젯 클래스는 BP에서 지정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

private:
	bool bIsDead = false;
};
