// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "DSAIController.h"
#include "DSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

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


	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
			if (DSCharacter && DSCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADSAIController::TargetKey, DSCharacter);

				// 시점을 타겟에 고정하고 좌우 움직임
				ADSCharacter* Character = Cast<ADSCharacter>(ControllingPawn);
				Character->Target();



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
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADSAIController::TargetKey, nullptr);
	}



	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}