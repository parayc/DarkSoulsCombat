// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_RunAttackDistance.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_RunAttackDistance::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
		return false;

	ADSCharacter* pDSCharacter = Cast<ADSCharacter>(ControllingPawn);



	float fDistanceToTarget = Target->GetDistanceTo(ControllingPawn);

	if (fDistanceToTarget >= 400.0f)
	{
		bResult = true;
	}
	else
	{

		bResult = false;
	//	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
	}
	

	return bResult;
}