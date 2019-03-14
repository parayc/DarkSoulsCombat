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

	void PlayHitReactionFront();
	void PlayHitReactionBack();
	void PlayHitReactionLeft();
	void PlayHitReactionRight();

	// 콤보 공격관련 함수
	void JumpToAttackMontageSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

	FOnAttackHitCheckDelegate	OnAttackHit;
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnLastAttackDelegate		OnLastAttack;

	int m_nAttackComboType;
	void SetAttackComboType(int nAttackComboType);

	void SetHitDirection(float fValue);
	void SetDeadAnim(bool bValue);
	void SetRunInputCheck(bool bValue);
	void PlayRollMontage();
	bool IsRolling();
	bool FunctionIsDead();

	bool GetRunInputCheck();

	bool AutoAttackCombo(int nAttackComboType);


	/*void InitializeAnimation();
*/
private:

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fHitDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bRunInputCheck;

	// 다음 콤보로 이어질 수 있는 구간인 NextAttackCheck 노티파이 발생시 호출 함수
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_LastAttack();

	UFUNCTION()
	void AnimNotify_RightPlant();

	UFUNCTION()
	void AnimNotify_LeftPlant();


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_old;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Combo01;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Combo02;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Combo03;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Combo04;




	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactionFront;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactionBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactionLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactionRight;

	// Rolling
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollBackward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollBackwardLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollBackwardRight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollForward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollForwardLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollForwardRight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DodgeRoll, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollRight;



};
