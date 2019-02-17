// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DSFootStepNotify.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UDSFootStepNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
