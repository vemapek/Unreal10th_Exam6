// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;

/**
 * 플레이어 HUD (화면 좌측 상단의 Health / Mana 표시)
 * UMG(WBP_PlayerHUD)에서 아래 이름의 위젯을 만들어야 연결된다.
 * HealthProgressBar, HealthText, ManaProgressBar, ManaText
 */
UCLASS()
class UNREAL10TH_EXAM6_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이 액터의 ASC를 찾아서 Health, Mana 변화를 감시한다.
	UFUNCTION(BlueprintCallable)
	void InitializeWithAbilitySystem(AActor* InActor);

protected:
	virtual void NativeDestruct() override;

	// 어트리뷰트 값이 바뀔 때 호출되는 콜백들
	void OnHealthChanged(const FOnAttributeChangeData& InData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& InData);
	void OnManaChanged(const FOnAttributeChangeData& InData);
	void OnMaxManaChanged(const FOnAttributeChangeData& InData);

	void UpdateHealthUI();
	void UpdateManaUI();

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UProgressBar> ManaProgressBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> ManaText;

private:
	float CurrentHealth = 0.0f;
	float MaxHealth = 100.0f;
	float CurrentMana = 0.0f;
	float MaxMana = 100.0f;

	TWeakObjectPtr<UAbilitySystemComponent> ASC;
};
