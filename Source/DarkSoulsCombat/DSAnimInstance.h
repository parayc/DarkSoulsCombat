// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Animation/AnimInstance.h"
#include "DSAnimInstance.generated.h"

// 이건 왜케 안외워지냐 
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLastAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnParryingTimeStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnParryingTimeEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnParryingCheckDelegate);

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
	void PlayJumpAttackMontage();


	void PlayHitReactionFront();
	void PlayHitReactionBack();
	void PlayHitReactionLeft();
	void PlayHitReactionRight();

	void PlayParryingHitMontage();

	// 콤보 공격관련 함수
	void JumpToAttackMontageSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

	FOnAttackHitCheckDelegate		OnAttackHit;
	FOnNextAttackCheckDelegate		OnNextAttackCheck;
	FOnLastAttackDelegate			OnLastAttack;
	FOnParryingTimeStartDelegate	OnParryingTimeStart;
	FOnParryingTimeEndDelegate		OnParryingTimeEnd;
	FOnParryingCheckDelegate		OnParryingCheck;


	int m_nAttackComboType;
	void SetAttackComboType(int nAttackComboType);

	void SetHitDirection(float fValue);
	void SetDeadAnim(bool bValue);
	void SetRunInputCheck(bool bValue);
	void SetGuardInputCheck(bool bValue);

	void PlayShieldBlockMontage();

	void PlayRollMontage();
	bool IsRolling();
	bool FunctionIsDead();

	bool GetRunInputCheck();
	bool GetGuardInputCheck();

	bool AutoAttackCombo(int nAttackComboType);

	void PlayParryingMontage();

	UAnimMontage* GetRollBackward();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bGuardInputCheck;




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

	UFUNCTION()
	void AnimNotify_ParryingTimeStart();
	
	UFUNCTION()
	void AnimNotify_ParryingTimeEnd();
	
	UFUNCTION()
	void AnimNotify_ParryingCheck();


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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpAttackMontage_Combo01;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Parrying, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingMontage;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Parrying, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingHitMontage;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Guard, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ShieldBlockMontage;



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
