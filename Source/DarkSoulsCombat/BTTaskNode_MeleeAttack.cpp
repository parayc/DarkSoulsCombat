// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_MeleeAttack.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSAnimInstance.h"

UBTTaskNode_MeleeAttack::UBTTaskNode_MeleeAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}

	DSCharacter->SetAttackComboType(2);

	DSCharacter->Attack();
	DSCharacter->bPressedJump = true;
	DSCharacter->GetCharacterMovement()->JumpZVelocity = 400.0f;
	IsAttacking = true;

	DSCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		EBTNodeResult::Failed;
	}

	// ���� ������ �׽�Ʈ ���� �˷��ִ� �Լ� ȣ��~
	if (!IsAttacking)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::AICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}