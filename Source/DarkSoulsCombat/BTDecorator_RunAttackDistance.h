// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RunAttackDistance.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTDecorator_RunAttackDistance : public UBTDecorator
{
	GENERATED_BODY()

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	
};
