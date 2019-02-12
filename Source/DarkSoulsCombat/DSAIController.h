// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "AIController.h"
#include "DSAIController.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API ADSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADSAIController();
	virtual void Possess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
	
};
