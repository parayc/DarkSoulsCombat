// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_StopRun.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTTaskNode_StopRun : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTaskNode_StopRun();

	virtual	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
