// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandomMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTDecorator_RandomMeleeAttack : public UBTDecorator
{
	GENERATED_BODY()

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

};
