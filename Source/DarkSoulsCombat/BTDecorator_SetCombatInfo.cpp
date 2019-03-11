// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_SetCombatInfo.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_SetCombatInfo::UBTDecorator_SetCombatInfo()
{

}


bool UBTDecorator_SetCombatInfo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
		return false;
	

	int nTemp = rand() % 5;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ADSAIController::ObserveTimeKey, nTemp);



	return bResult;
}