// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimSlowStartNotify.h"


void UDSAnimSlowStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Animation->RateScale = 0.4f;
}