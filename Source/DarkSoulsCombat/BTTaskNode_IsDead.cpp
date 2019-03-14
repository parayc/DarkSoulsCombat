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

	// ���� �̰� �ƴѰͰ����� �ϴ� �̷���... ������ �ǵ���
	//// ��������Ʈ�� �޾Ƽ� ����� �����̺�Ʈ�� ���� ���ϵ��� ����
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
