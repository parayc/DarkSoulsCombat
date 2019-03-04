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
	
	// ������ Ÿ���� �������� ai������ ���� Ai move target to around �� ���� �ƴϸ� �� ������
	//OwnerComp.GetBlackboardComponent()->SetValueAsInt(ADSAIController::RandomKey, nSelRandom);

	int nAttackOrAround = rand() % 3;

	if (nAttackOrAround == 2)
	{
		int nDirRandomSel = rand() % 4;

		FVector vecCurrentLocation = ControllingPawn->GetActorLocation();

		switch (nDirRandomSel)
		{
			// ��
		case 0:
		{
			//FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll);

			//FORCEINLINE FVector::FVector(float InX, float InY, float InZ);

			FVector vecMoveLocation = vecCurrentLocation + FVector(300, 0, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// ��
		case 1:
		{
			FVector vecMoveLocation = vecCurrentLocation + FVector(-300, 0, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// ��
		case 2:
		{
			FVector vecMoveLocation = vecCurrentLocation + FVector(0, 300, 0);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::AroundPosKey, vecMoveLocation);

			return false;
			break;
		}

		// ��
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
