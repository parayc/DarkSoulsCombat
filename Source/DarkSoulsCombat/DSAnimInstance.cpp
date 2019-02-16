// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimInstance.h"

UDSAnimInstance::UDSAnimInstance()
{
	float fCurrentPawnSpeed = 0.0f;
	float fDirection = 0.0f;
	bool bIsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactor_AttackMontage.DSCharactor_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ƽ���� �� ��������
	APawn* pPawn = TryGetPawnOwner();

	if (::IsValid(pPawn))
	{
		fCurrentPawnSpeed = pPawn->GetVelocity().Size();
		ACharacter* pCharacter = Cast<ACharacter>(pPawn);

		// �ٶ󺸴� ����� �����̴� ������ ���� ������ ��ȯ����
		fDirection = CalculateDirection(pCharacter->GetVelocity(), pCharacter->GetActorRotation());

		// ���߿� ���ִ��� Ȯ��
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

void UDSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//DSCHECK(Montage_IsPlaying(AttackMontage))

	// ���� ��Ÿ�� �������� �Ѿ��
	// �Ű����� (�Ѿ ���� ���ڿ�, �ش� ��Ÿ��)
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UDSAnimInstance::AnimNotify_AttackHit()
{
	DSLOG_S(Warning);

	OnAttackHit.Broadcast();
}

void UDSAnimInstance::AnimNotify_NextAttackCheck()
{
	DSLOG_S(Warning);

	OnNextAttackCheck.Broadcast();
}

// Attack1~3 ������ �����̸��� �´� ���� ���� ���� �������ִ� �Լ�
FName UDSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	DSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	// Attack1~3 ������ �����̸��� �´� ���� ���� ���� �������ִ� �Լ�
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}