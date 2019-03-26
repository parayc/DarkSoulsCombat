// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_RandomMeleeAttack.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSAnimInstance.h"


bool UBTDecorator_RandomMeleeAttack::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	int nMeleeAttackType = rand() % 2 + 1;
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ADSAIController::nMeleeAttackTypeKey, nMeleeAttackType);

	return bResult;
}