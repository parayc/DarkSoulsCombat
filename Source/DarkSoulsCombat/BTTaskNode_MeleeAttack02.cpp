// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_MeleeAttack02.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSAnimInstance.h"

UBTTaskNode_MeleeAttack02::UBTTaskNode_MeleeAttack02()
{
	bNotifyTick = true;
	IsAttacking = false;
	m_nAttackCnt = 0;
	m_nRandParrying = 0;
}

EBTNodeResult::Type UBTTaskNode_MeleeAttack02::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}

	int nMeleeAttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(ADSAIController::nMeleeAttackTypeKey);
	
	if (nMeleeAttackType != 2)
	{
		return EBTNodeResult::Failed;
	}
	
	if (!DSCharacter->IsPlayerControlled())
	{
		DSCharacter->SetAttackComboType(nMeleeAttackType);
	}
	m_nAttackCnt = rand() % 3 + 1;




	DSCharacter->Attack();
	IsAttacking = true;

	DSCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_MeleeAttack02::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		DSCharacter->SetAttackComboType(1);
		EBTNodeResult::Failed;
	}

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
	{
		DSCharacter->SetAttackComboType(1);
		EBTNodeResult::Failed;
	}

	if (DSCharacter->GetDistanceTo(Target) > 200)
	{
		DSCharacter->SetAttackComboType(1);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}


	if (Target->IsAttacking)
	{
		if (m_nRandParrying == 0)
		{
			DSCharacter->OnParrying();
			DSCharacter->SetAttackComboType(1);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	// AI�� �޺������� ���� �Լ�
	FName CurrentSection = DSCharacter->GetDSAnim()->Montage_GetCurrentSection();

	FString str = CurrentSection.ToString();
	FString strTemp = str.Right(1);

	// ���ǰ� ��Ʈ�� ����
	// FCString::Atoi mfc �����̶� ����ϳ� atoi atof ���°�
	int nSectionNum = FCString::Atoi(*strTemp);


	// ���ǰ��� ���ٸ�
	if (nSectionNum != 0 && nSectionNum < m_nAttackCnt)
	{
		DSCharacter->Attack();
	}


	// ���� ������ �׽�Ʈ ���� �˷��ִ� �Լ� ȣ��~
	if (!IsAttacking)
	{
		DSCharacter->SetAttackComboType(1);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
