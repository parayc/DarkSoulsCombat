
// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_StartRun.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_StartRun::UBTTaskNode_StartRun()
{

}

EBTNodeResult::Type UBTTaskNode_StartRun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::RunTargetPosKey, (Target->GetActorLocation() - DSCharacter->GetActorForwardVector()));

	DSCharacter->StartRun();
	
	return EBTNodeResult::Succeeded;
}

//void UBTTaskNode_StartRun::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//
//	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
//	if (nullptr == Target)
//	{
//		return EBTNodeResult::Failed;
//	}
//
//	ADSCharacter* pDSCharacter = Cast<ADSCharacter>(ControllingPawn);
//
//
//
//	float fDistanceToTarget = Target->GetDistanceTo(ControllingPawn);
//
//
//	// 어택 끝나면 테스트 종료 알려주는 함수 호출~
//	if ()
//	{
//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	}
//}
