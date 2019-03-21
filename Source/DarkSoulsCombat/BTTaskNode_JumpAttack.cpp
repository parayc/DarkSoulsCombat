// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_JumpAttack.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSAnimInstance.h"


UBTTaskNode_JumpAttack::UBTTaskNode_JumpAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}

	//DSCharacter->SetAttackComboType(2);

	DSCharacter->bPressedJump = true;
	// �׳� Attack���� �ϸ� ���߿� �߱⵵ ���� ATTACK�� ���������ֳ���
	DSCharacter->JumpAttack();
	DSCharacter->GetCharacterMovement()->JumpZVelocity = 400.0f;
	IsAttacking = true;

	DSCharacter->OnAttackEnd.AddLambda([=]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_JumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}