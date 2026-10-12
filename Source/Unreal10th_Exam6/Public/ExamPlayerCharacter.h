// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ExamPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAbilitySystemComponent;
class UPlayerAttributeSet;
class UInputAction;
struct FInputActionValue;

/**
 * 플레이어 캐릭터
 * ASC(AbilitySystemComponent)와 PlayerAttributeSet을 가지고 있다.
 */
UCLASS()
class UNREAL10TH_EXAM6_API AExamPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AExamPlayerCharacter();

	// IAbilitySystemInterface : 다른 코드가 이 캐릭터의 ASC를 찾을 때 사용
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }

protected:
	// 컨트롤러에 빙의되었을 때 ASC 초기화
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

protected:
	// ---------------- GAS ----------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

	// ---------------- 카메라 ----------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	// ---------------- 입력 (BP에서 지정) ----------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// 마우스 시점용 (템플릿에서 IA_Look과 IA_MouseLook은 따로 있다)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
};
