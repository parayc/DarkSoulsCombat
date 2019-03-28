// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimSlowEndNotify.h"


void UDSAnimSlowEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Animation->RateScale = 1.0f;
}