// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	int nAttackCnt;
	int m_nAttackComboType;
	int m_nRandParrying;

	bool IsAttacking;
	
};
