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

	//// 람다에서 지역변수 쓰려고 [=] 지역변수 참조는 [&]
	//DSCharacter->GetDSAnim()->OnNextAttackCheck.AddLambda([=]() -> void {
	//	// 여기에 이제 공격 섹션이 Attack1~4로 통일된 이름이니까
	//	// 현재 섹션이 1이면 2를 실행하고 2면 3을 실행하고 4면 4를 실행하고

	//	FName CurrentSection = DSCharacter->GetDSAnim()->Montage_GetCurrentSection();

	//	FString str = CurrentSection.ToString();
	//	FString strTemp = str.Right(1);

	//	/*DSLOG(Warning, TEXT("StreamTest : %s "), *strTemp);*/
	//	
	//	// 섹션값 인트로 변경
	//	// FCString::Atoi mfc 변경이랑 비슷하네 atoi atof 쓰는건
	//	int nSectionNum = FCString::Atoi(*strTemp);

	//	// 섹션값이 낮다면
	//	if (nSectionNum < nAttackCnt)
	//	{
	//		nSectionNum++;

	//		FString strNextSectionNum = "";

	//		// int를 FString으로
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


	// AI의 콤보공격을 위한 함수
	FName CurrentSection = DSCharacter->GetDSAnim()->Montage_GetCurrentSection();

	FString str = CurrentSection.ToString();
	FString strTemp = str.Right(1);

	// 섹션값 인트로 변경
	// FCString::Atoi mfc 변경이랑 비슷하네 atoi atof 쓰는건
	int nSectionNum = FCString::Atoi(*strTemp);


	// 섹션값이 낮다면
	if (nSectionNum != 0 && nSectionNum < nAttackCnt)
	{
		DSCharacter->Attack();
	}


	//DSCharacter->Attack();

	// 어택 끝나면 테스트 종료 알려주는 함수 호출~
	if (!IsAttacking)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}