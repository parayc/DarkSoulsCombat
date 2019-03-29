// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_BBBase_StunCheck.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_BBBase_StunCheck::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	ADSCharacter* pDSCharacter = Cast<ADSCharacter>(ControllingPawn);

	pDSCharacter->OnSetStunDelegate.AddLambda([&OwnerComp, &pDSCharacter]() -> void {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsStunKey, pDSCharacter->IsStun());
	});


	/*pDSCharacter->OnSetStunDelegate.AddDynamic(this, &UBTDecorator_BBBase_StunCheck::SetStunKey);
	*/
	return bResult;
}


EBlackboardNotificationResult UBTDecorator_BBBase_StunCheck::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	EBlackboardNotificationResult eResult = Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);

	bool IsStun = Blackboard.GetValueAsBool(ADSAIController::IsStunKey);
	
	IsStun ? eResult = EBlackboardNotificationResult::ContinueObserving : eResult = EBlackboardNotificationResult::RemoveObserver;

	return eResult;
}

void UBTDecorator_BBBase_StunCheck::SetStunKey(bool Token)
{
	
}