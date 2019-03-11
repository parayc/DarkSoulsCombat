// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_StopRun.h"
#include "DSCharacter.h"
#include "DSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_StopRun::UBTTaskNode_StopRun()
{

}

EBTNodeResult::Type UBTTaskNode_StopRun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto DSCharacter = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == DSCharacter)
	{
		return EBTNodeResult::Failed;
	}

	if (DSCharacter->GetRunInputCheck())
	{
		DSCharacter->StopRun();
	}

	return EBTNodeResult::Succeeded;
}