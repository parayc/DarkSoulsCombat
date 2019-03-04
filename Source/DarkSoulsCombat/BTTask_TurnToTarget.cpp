// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FVector LookVector = Target->GetActorLocation() - DSCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	DSCharacter->SetActorRotation(FMath::RInterpTo(DSCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));

	return EBTNodeResult::Succeeded;
}