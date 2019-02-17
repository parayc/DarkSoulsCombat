// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Animation/AnimInstance.h"
#include "DSAnimInstance.generated.h"

// 이건 왜케 안외워지냐 
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLastAttackDelegate);

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UDSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDSAnimInstance();

	//틱마다 호출됨 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	// 콤보 공격관련 함수
	void JumpToAttackMontageSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

	FOnAttackHitCheckDelegate	OnAttackHit;
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnLastAttackDelegate		OnLastAttack;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fCurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fDirection;

	// 다음 콤보로 이어질 수 있는 구간인 NextAttackCheck 노티파이 발생시 호출 함수
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_LastAttack();


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;


};
