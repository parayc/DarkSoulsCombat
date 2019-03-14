// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_IsDead.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "DSCharacterStatComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_IsDead::UBTTaskNode_IsDead()
{

}

EBTNodeResult::Type UBTTaskNode_IsDead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	// 뭔가 이거 아닌것같지만 일단 이렇게... 동작은 되도록
	//// 델리게이트를 받아서 사망시 비헤이비트리 동작 못하도록 막음
	pOwnerComp = &OwnerComp;

	DSCharacter->CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		pOwnerComp->GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsDeadKey, true);
	});

	if (!(pOwnerComp->GetBlackboardComponent()->GetValueAsBool(ADSAIController::IsDeadKey)))
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::RunTargetPosKey, (Target->GetActorLocation() - DSCharacter->GetActorForwardVector()));


	return EBTNodeResult::Succeeded;
}
