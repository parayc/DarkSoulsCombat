// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	nAttackCnt = 0;
	IsAttacking = false;
	m_nAttackComboType = 1;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}
	
	int nMeleeAttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(ADSAIController::nMeleeAttackTypeKey);

	if (nMeleeAttackType != 1)
	{
		return EBTNodeResult::Failed;
	}

	DSCharacter->SetAttackComboType(nMeleeAttackType);

	DSCharacter->Attack();
	IsAttacking = true;

	nAttackCnt = rand() % 4 + 1;




	//DSCharacter->GetDSAnim()->SectionEnd

	//// ���ٿ��� �������� ������ [=] �������� ������ [&]
	//DSCharacter->GetDSAnim()->OnNextAttackCheck.AddLambda([=]() -> void {
	//	// ���⿡ ���� ���� ������ Attack1~4�� ���ϵ� �̸��̴ϱ�
	//	// ���� ������ 1�̸� 2�� �����ϰ� 2�� 3�� �����ϰ� 4�� 4�� �����ϰ�

	//	FName CurrentSection = DSCharacter->GetDSAnim()->Montage_GetCurrentSection();

	//	FString str = CurrentSection.ToString();
	//	FString strTemp = str.Right(1);

	//	/*DSLOG(Warning, TEXT("StreamTest : %s "), *strTemp);*/
	//	
	//	// ���ǰ� ��Ʈ�� ����
	//	// FCString::Atoi mfc �����̶� ����ϳ� atoi atof ���°�
	//	int nSectionNum = FCString::Atoi(*strTemp);

	//	// ���ǰ��� ���ٸ�
	//	if (nSectionNum < nAttackCnt)
	//	{
	//		nSectionNum++;

	//		FString strNextSectionNum = "";

	//		// int�� FString����
	//		strNextSectionNum.FString::FromInt(nSectionNum);

	//		if (strNextSectionNum != "")
	//		{
	//			DSCharacter->GetDSAnim()->JumpToAttackMontageSection(nSectionNum);
	//		}
	//	}

	//});

	DSCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});


	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		EBTNodeResult::Failed;
	}

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
	{
		EBTNodeResult::Failed;
	}

	if (DSCharacter->GetDistanceTo(Target) > 200)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}


	// AI�� �޺������� ���� �Լ�
	FName CurrentSection = DSCharacter->GetDSAnim()->Montage_GetCurrentSection();

	FString str = CurrentSection.ToString();
	FString strTemp = str.Right(1);

	// ���ǰ� ��Ʈ�� ����
	// FCString::Atoi mfc �����̶� ����ϳ� atoi atof ���°�
	int nSectionNum = FCString::Atoi(*strTemp);


	// ���ǰ��� ���ٸ�
	if (nSectionNum != 0 && nSectionNum < nAttackCnt)
	{
		DSCharacter->Attack();
	}


	//DSCharacter->Attack();

	// ���� ������ �׽�Ʈ ���� �˷��ִ� �Լ� ȣ��~
	if (!IsAttacking)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}