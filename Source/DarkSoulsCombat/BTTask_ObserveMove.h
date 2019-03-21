// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "DSCharacter.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ObserveMove.generated.h"

UENUM(BlueprintType)
enum class EAICombatState : uint8
{
	ObservedMode UMETA(DisplayName = "ObservedMode"),
	AttackMode UMETA(DisplayName = "AttackMode"),
};


/**
 * 
 */
enum class MOVE_DIR{
	eDir_FB = 0,
	eDir_LR = 1,
};


UCLASS()
class DARKSOULSCOMBAT_API UBTTask_ObserveMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ObserveMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	ADSCharacter* Character;

	ADSCharacter* m_Target;

	// 플레이 시간
	int nPlayTimeCnt;

	float DeltaSecSum;

	int nAxisValue;

	MOVE_DIR eMoveDir;
};
