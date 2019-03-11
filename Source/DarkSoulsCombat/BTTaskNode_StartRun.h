// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_StartRun.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTTaskNode_StartRun : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UBTTaskNode_StartRun();

	virtual	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/*virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
*/
};
