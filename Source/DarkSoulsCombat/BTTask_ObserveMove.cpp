// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ObserveMove.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ObserveMove::UBTTask_ObserveMove()
{
	bNotifyTick = true;

	nPlayTimeCnt = 0;
	nAxisValue = 0;
	DeltaSecSum = 0;
}

EBTNodeResult::Type UBTTask_ObserveMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ADSCharacter* Character = Cast<ADSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 0);
	//
	//if (Character->CameraTarget == nullptr)
	//{
	//	return EBTNodeResult::Succeeded;
	//}

	nPlayTimeCnt = rand() % 5;
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

	// 랜덤 카운트가 0이되었으니까 이제 빠져나간다
	if (nPlayTimeCnt <= 0)
	{
		//nPlayTimeCnt = 5;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ADSAIController::eAICombatStateKey, 1);
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

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		Character->AddMovementInput(Direction, nAxisValue);
		// 이동 방향 끝 
	}
}