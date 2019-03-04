// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	if (nullptr == Target)
		return false;
	
	// 범위에 타겟이 들어왔을대 ai공격을 할지 Ai move target to around 를 할지 아니면 방어를 취할지
	//OwnerComp.GetBlackboardComponent()->SetValueAsInt(ADSAIController::RandomKey, nSelRandom);

	int nAttackOrAround = rand() % 3;

	if (nAttackOrAround == 2)
	{
		int nDirRandomSel = rand() % 4;

		FVector vecCurrentLocation = ControllingPawn->GetActorLocation();

		switch (nDirRandomSel)
		{
			// 상
		case 0:
		{
			//FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll);

			//FORCEINLINE FVector::FVector(float InX, float InY, float InZ);

			FVector vecMoveLocation = vecCurrentLocation + FVector(300, 0, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// 하
		case 1:
		{
			FVector vecMoveLocation = vecCurrentLocation + FVector(-300, 0, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// 좌
		case 2:
		{
			FVector vecMoveLocation = vecCurrentLocation + FVector(0, 300, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// 우
		case 3:
		{
			FVector vecMoveLocation = vecCurrentLocation + FVector(0, -300, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		default:
			break;
		}

	}


	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
