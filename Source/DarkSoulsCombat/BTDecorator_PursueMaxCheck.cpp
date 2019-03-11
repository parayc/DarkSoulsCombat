// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_PursueMaxCheck.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


bool UBTDecorator_PursueMaxCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
		return false;

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ADSAIController::IsPursueMaxRangeKey))
	{
		bResult = false;
	}
	else
	{
		bResult = true;
	}

	return bResult;
}