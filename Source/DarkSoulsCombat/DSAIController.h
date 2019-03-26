// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "AIController.h"
#include "DSAIController.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API ADSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADSAIController();
	virtual void Possess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName RandomKey;
	static const FName AroundPosKey;
	static const FName RunTargetPosKey;
	static const FName fDistanceToTargetKey;
	static const FName PursueHomePosKey;
	static const FName DistanceFromPursueHomePosToTargetKey;
	static const FName IsPursueMaxRangeKey;
	static const FName AICombatStateKey;
	static const FName ObserveTimeKey;
	static const FName AttackCountKey;
	static const FName AIStateKey;
	static const FName eAICombatStateKey;
	static const FName fDeltaSecSumKey;
	static const FName IsDeadKey;
	static const FName nMeleeAttackTypeKey;



private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
	
};
