// Fill out your copyright notice in the Description page of Project Settings.

#include "DSSwingSoundNotify.h"
#include "DSCharacter.h"

void UDSSwingSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ADSCharacter* Character = Cast<ADSCharacter>(MeshComp->GetOwner());
		if (Character != NULL)
		{
			Character->AttackAudioComponent->Play(0.f);
		}
	}

}