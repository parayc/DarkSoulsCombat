// Fill out your copyright notice in the Description page of Project Settings.

#include "DSRollAnimNotify.h"
#include "DSCharacter.h"
#include "Runtime/Engine//Classes/Kismet/KismetMathLibrary.h"

void UDSRollAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	//{
	//	ADSCharacter* Character = Cast<ADSCharacter>(MeshComp->GetOwner());
	//	if (Character != NULL)
	//	{
	//		//Character->DSAnim->FootStepAudioComponent->Play(0.f);
	//		Character->SetActorLocation();
	//	}
	//}

	ADSCharacter* Character = Cast<ADSCharacter>(MeshComp->GetOwner());

	FVector vecTemp;
	vecTemp.X = Character->InputComponent->GetAxisValue(TEXT("MoveForward"));
	vecTemp.Y = Character->InputComponent->GetAxisValue(TEXT("MoveRight"));
	vecTemp.Z = 0;

	UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), vecTemp);

	FRotator rotTemp;

	//CalculateDirection();
	//

	//InverseTransfoD
}