// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_IsDead.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTTaskNode_IsDead : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_IsDead();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UBehaviorTreeComponent* pOwnerComp;
};
