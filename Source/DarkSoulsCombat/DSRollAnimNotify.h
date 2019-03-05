// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DSRollAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UDSRollAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
