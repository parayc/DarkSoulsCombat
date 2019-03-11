// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AICombat.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UBTService_AICombat : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AICombat();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	int m_nPlayTimeCnt;
	int m_DeltaSecSum;
	int m_nObserveTime;
	int m_nAttackCount;
	FVector m_vecPursueHomePos;
};
