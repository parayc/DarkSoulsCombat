// Fill out your copyright notice in the Description page of Project Settings.

#include "DSFootStepNotify.h"
#include "DSCharacter.h"

void UDSFootStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ADSCharacter* Character = Cast<ADSCharacter>(MeshComp->GetOwner());
		if (Character != NULL)
		{
			Character->FootStepAudioComponent->Play(0.f);
		}
	}

}