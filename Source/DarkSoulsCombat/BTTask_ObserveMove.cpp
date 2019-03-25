// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ObserveMove.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "DSAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ObserveMove::UBTTask_ObserveMove()
{
	bNotifyTick = true;

	nPlayTimeCnt = 0;
	nAxisValue = 0;
	DeltaSecSum = 0;
	m_nGuard = 0;
	eMoveDir = MOVE_DIR::eDir_FB;
}

EBTNodeResult::Type UBTTask_ObserveMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Character = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	m_Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);

	nPlayTimeCnt = rand() % 5;
	int nDir = rand() % 2;
	m_nGuard = rand() % 4;

	if (m_nGuard != 0)
	{
		Character->StartGuard();
	}

	switch (nDir)
	{
		case 0:
		{
			eMoveDir = MOVE_DIR::eDir_FB;
			break;
		}
		case 1:
		{
			eMoveDir = MOVE_DIR::eDir_LR;
			break;
		}
	}


	nAxisValue = (rand() % 2) == 0 ? nAxisValue = -1 : nAxisValue = 1;


	return EBTNodeResult::InProgress;
}

void UBTTask_ObserveMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	DeltaSecSum += DeltaSeconds;

	if (DeltaSecSum >= 1)
	{
		nPlayTimeCnt--;
		DSLOG(Warning, TEXT("%d"), nPlayTimeCnt);
		DeltaSecSum = 0;
	}
	
	float fDistanceToTarget = Character->GetDistanceTo(m_Target);
	if (fDistanceToTarget <= 200.0f)
	{
		int nRandomRoll = rand() % 3;

		if (nRandomRoll == 0)
		{
			Character->GetDSAnim()->Montage_Play(Character->GetDSAnim()->GetRollBackward(), 1.0f);
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 1);
		Character->StopGuard();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	// 랜덤 카운트가 0이되었으니까 이제 빠져나간다
	if (nPlayTimeCnt <= 0)
	{
		//nPlayTimeCnt = 5;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 1);
		Character->StopGuard();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// 어라운드 이동
		ADSCharacter* Character = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());

		auto Target = Cast<ADSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey));
		if (nullptr == Target)
		{
			return;
		}
		Character->bUseControllerRotationYaw = false;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;

		FVector TargetVect = Target->GetActorLocation() - Character->GetActorLocation();
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = Character->GetActorRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 10.f);

		Character->SetActorRotation(NewRot);


		// 이동 방향
		const FRotator Rotation = (Target->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal().Rotation();
		//const FRotator Rotation = Character->CameraTarget == nullptr ? Character->GetControlRotation() : (Character->CameraTarget->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal().Rotation();
		

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction;

		switch (eMoveDir)
		{
			case MOVE_DIR::eDir_FB:
			{
				Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				nAxisValue = 1;

				break;
			}

			case MOVE_DIR::eDir_LR:
			{
				Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

				break;
			}
		}

		Character->AddMovementInput(Direction, nAxisValue);
		// 이동 방향 끝 
	}
}