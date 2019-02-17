// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Animation/AnimInstance.h"
#include "DSAnimInstance.generated.h"

// �̰� ���� �ȿܿ����� 
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

	//ƽ���� ȣ��� 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	// �޺� ���ݰ��� �Լ�
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

	// ���� �޺��� �̾��� �� �ִ� ������ NextAttackCheck ��Ƽ���� �߻��� ȣ�� �Լ�
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_LastAttack();


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;


};
