// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetCombatInfo.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTDecorator_SetCombatInfo : public UBTDecorator
{
	GENERATED_BODY()



private:
	UBTDecorator_SetCombatInfo();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
