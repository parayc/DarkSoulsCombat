// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DSCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.3f;
	vecPursueHomePosKey = FVector::ZeroVector;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	ADSCharacter* Character = Cast<ADSCharacter>(ControllingPawn);

	// ���� �̰� �ƴѰͰ����� �ϴ� �̷���... ������ �ǵ���
	// ��������Ʈ�� �޾Ƽ� ����� �����̺�Ʈ�� ���� ���ϵ��� ����
	pOwnerComp = &OwnerComp;

	Character->CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		pOwnerComp->GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsDeadKey, true);
	});

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 800.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	float fDistanceFromPursueHomePosToTarget = (ControllingPawn->GetActorLocation() - vecPursueHomePosKey).Size();

	if (fDistanceFromPursueHomePosToTarget >= 1500.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsPursueMaxRangeKey, true);

		UObject* DSCharacter = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADSAIController::TargetKey);

		if (DSCharacter != nullptr)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADSAIController::TargetKey, nullptr);
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::IsPursueMaxRangeKey, false);
	}

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
			if (DSCharacter && DSCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADSAIController::TargetKey, DSCharacter);

				// ������ Ÿ�ٿ� �����ϰ� �¿� ������
			
				Character->Target();

				if (vecPursueHomePosKey.Size() == 0)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::PursueHomePosKey, ControllingPawn->GetActorLocation());
					vecPursueHomePosKey = ControllingPawn->GetActorLocation();
				}

				//ture �� combat
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::AIStateKey, true);

				

				//Character->bUseControllerRotationYaw = true;
				//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
				//Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, DSCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), DSCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	else
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADSAIController::TargetKey, nullptr);
		Character->Target();
		Character->SetControlMode(EControlMode::eNomal);
		vecPursueHomePosKey = FVector::ZeroVector;
		// Patrol
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADSAIController::AIStateKey, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADSAIController::PursueHomePosKey, vecPursueHomePosKey);
	}





	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}