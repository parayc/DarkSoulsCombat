// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_AICombat.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AICombat::UBTService_AICombat()
{
	m_nPlayTimeCnt = 0;
	m_DeltaSecSum = 0;
	m_nObserveTime = rand() % 5;
	m_nAttackCount = rand() % 5;
}

void UBTService_AICombat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* pBlackboardComponent = OwnerComp.GetBlackboardComponent();

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	auto Target = Cast<ADSCharacter>(pBlackboardComponent->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
	{
		return;
	}

	ADSCharacter* pDSCharacter = Cast<ADSCharacter>(ControllingPawn);

	// Key Set
	// TargetDistance
	float fDistanceToTarget = Target->GetDistanceTo(ControllingPawn);
	pBlackboardComponent->SetValueAsFloat(ADSAIController::fDistanceToTargetKey, fDistanceToTarget);


	pBlackboardComponent->SetValueAsBool(ADSAIController::AICombatStateKey, true);


	// AICombatState
	// 시간 카운트해서~

	float DeltaSecSum = pBlackboardComponent->GetValueAsFloat(ADSAIController::fDeltaSecSumKey);
	
	if (DeltaSecSum >= 1)
	{

		pBlackboardComponent->SetValueAsFloat(ADSAIController::fDeltaSecSumKey, 0);
	}

	//DeltaSecSum += DeltaSeconds;

	//pBlackboardComponent->SetValueAsFloat(ADSAIController::fDeltaSecSumKey, DeltaSecSum);

	//
	//if (m_DeltaSecSum >= 1)
	//{
	//	pBlackboardComponent->SetValueAsFloat(ADSAIController::ObserveTimeKey, m_nObserveTime);
	//	m_nObserveTime--;
	//	m_DeltaSecSum = 0;
	//}

	//if (m_nObserveTime <= 0)
	//{
	//	pBlackboardComponent->SetValueAsBool(ADSAIController::AICombatStateKey, false);
	//}
	//else
	//{
	//	pBlackboardComponent->SetValueAsBool(ADSAIController::AICombatStateKey, true);
	//}

	//float fDistanceFromPursueHomePosToTarget = (ControllingPawn->GetActorLocation() - vecPursueHomePosKey).Size();

	// AttackCount
	pBlackboardComponent->SetValueAsBool(ADSAIController::AttackCountKey, m_nAttackCount);



	// IsPursueMaxRange
	m_vecPursueHomePos = pBlackboardComponent->GetValueAsVector(ADSAIController::PursueHomePosKey);

	//(ControllingPawn->GetActorLocation() - m_vecPursueHomePos);


	//pBlackboardComponent->SetValueAsBool(ADSAIController::IsPursueMaxRangeKey, m_vecPursueHomePos);


	//const FName ADSAIController::HomePosKey(TEXT("HomePos"));
	//const FName ADSAIController::PatrolPosKey(TEXT("PatrolPos"));
	//const FName ADSAIController::TargetKey(TEXT("Target"));
	//const FName ADSAIController::RandomKey(TEXT("Random"));
	//const FName ADSAIController::AroundPosKey(TEXT("AroundPos"));
	//const FName ADSAIController::RunTargetPosKey(TEXT("RunTargetPos"));
	//const FName ADSAIController::fDistanceToTargetKey(TEXT("fDistanceToTarget"));
	//const FName ADSAIController::PursueHomePosKey(TEXT("PursueHomePos"));
	//const FName ADSAIController::DistanceFromPursueHomePosToTargetKey(TEXT("DistanceFromPursueHomePosToTarget"));
	//const FName ADSAIController::IsPursueMaxRangeKey(TEXT("IsPursueMaxRange"));


			// 시야 방향
	pDSCharacter->bUseControllerRotationYaw = false;
	pDSCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	FVector TargetVect = Target->GetActorLocation() - pDSCharacter->GetActorLocation();
	FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
	FRotator CurrentRot = pDSCharacter->GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 10.f);

	pDSCharacter->SetActorRotation(NewRot);
	// 시햐 방향 끝
	//FVector LookVector = Target->GetActorLocation() - pDSCharacter->GetActorLocation();
	//LookVector.Z = 0.0f;
	//FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//pDSCharacter->SetActorRotation(FMath::RInterpTo(pDSCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));



	//pBlackboardComponent->SetValueAsFloat(ADSAIController::fDistanceToTargetKey, int n);

}