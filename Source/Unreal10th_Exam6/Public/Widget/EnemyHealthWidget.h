// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EnemyHealthWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;

/**
 * 적 머리 위에 표시되는 Health 위젯
 * UMG(WBP_EnemyHealth)에서 아래 이름의 위젯을 만들어야 연결된다.
 * HealthProgressBar (필수), HealthText (필수)
 */
UCLASS()
class UNREAL10TH_EXAM6_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이 액터의 ASC를 찾아서 Health 변화를 감시한다.
	UFUNCTION(BlueprintCallable)
	void InitializeWithAbilitySystem(AActor* InActor);

protected:
	virtual void NativeDestruct() override;

	void OnHealthChanged(const FOnAttributeChangeData& InData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& InData);
	void UpdateHealthUI();

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> HealthText;

private:
	float CurrentHealth = 0.0f;
	float MaxHealth = 100.0f;

	TWeakObjectPtr<UAbilitySystemComponent> ASC;
};
