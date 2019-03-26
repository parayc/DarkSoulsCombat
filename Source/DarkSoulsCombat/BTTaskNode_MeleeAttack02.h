// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MeleeAttack02.generated.h"

/**
 * 
 */

UCLASS()
class DARKSOULSCOMBAT_API UBTTaskNode_MeleeAttack02: public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_MeleeAttack02();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	bool IsAttacking;

	int m_nAttackCnt;

};

