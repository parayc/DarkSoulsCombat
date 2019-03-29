// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_BBBase_StunCheck.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTDecorator_BBBase_StunCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_BBBase_StunCheck();

private:

	//ADSCharacter* m_pDSCharacter;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;

	UFUNCTION()
	void SetStunKey(bool Token);

	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;

};
