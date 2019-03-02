// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName ADSAIController::HomePosKey(TEXT("HomePos"));
const FName ADSAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ADSAIController::TargetKey(TEXT("Target"));
const FName ADSAIController::RandomKey(TEXT("Random"));
const FName ADSAIController::AroundPosKey(TEXT("AroundPos"));

ADSAIController::ADSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/AI/BB_DSCharacter.BB_DSCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/AI/BT_DSCharacter.BT_DSCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

}

void ADSAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// AI컨트롤러가 조종하는 폰의 리스폰 위치를 블랙보드의 HomePosKey 변수에 저장
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{

		}
	}
}
