// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimInstance.h"

UDSAnimInstance::UDSAnimInstance()
{
	float fCurrentPawnSpeed = 0.0f;
	float fDirection = 0.0f;
	bool bIsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactorMontage.DSCharactorMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 틱에서 폰 가져오기
	APawn* pPawn = TryGetPawnOwner();

	if (::IsValid(pPawn))
	{
		fCurrentPawnSpeed = pPawn->GetVelocity().Size();
		ACharacter* pCharacter = Cast<ACharacter>(pPawn);

		// 바라보는 방향과 움직이는 방향의 사이 각도를 반환해줌
		fDirection = CalculateDirection(pCharacter->GetVelocity(), pCharacter->GetActorRotation());

		// 공중에 떠있는지 확인
		if (pCharacter)
		{
			bIsInAir = pCharacter->GetMovementComponent()->IsFalling();
		}
	}
}

void UDSAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}