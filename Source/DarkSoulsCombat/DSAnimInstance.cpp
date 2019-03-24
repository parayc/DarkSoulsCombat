// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimInstance.h"
#include "DSCharacter.h"

UDSAnimInstance::UDSAnimInstance()
{
	fCurrentPawnSpeed = 0.0f;
	fDirection = 0.0f;
	bIsInAir = false;
	IsDead = false;
	bRunInputCheck = false;


	
	// ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactor_AttackMontage.DSCharactor_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage_old = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_COMBO_01_MONTAGE(TEXT("/Game/Frank_RPG_Warrior/Animations/DS_Mannequin_RM/DS_Frank_RPG_Warrior_Combo01_Montage.DS_Frank_RPG_Warrior_Combo01_Montage"));
	if (ATTACK_COMBO_01_MONTAGE.Succeeded())
	{
		AttackMontage_Combo01 = ATTACK_COMBO_01_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_COMBO_02_MONTAGE(TEXT("/Game/Frank_RPG_Warrior/Animations/DS_Mannequin_RM/DS_Frank_RPG_Warrior_Attack04_Montage.DS_Frank_RPG_Warrior_Attack04_Montage"));
	if (ATTACK_COMBO_02_MONTAGE.Succeeded())
	{
		AttackMontage_Combo02 = ATTACK_COMBO_02_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_COMBO_03_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactor_AttackMontage.DSCharactor_AttackMontage"));
	if (ATTACK_COMBO_03_MONTAGE.Succeeded())
	{
		AttackMontage_Combo03 = ATTACK_COMBO_03_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_COMBO_04_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactor_AttackMontage.DSCharactor_AttackMontage"));
	if (ATTACK_COMBO_04_MONTAGE.Succeeded())
	{
		AttackMontage_Combo04 = ATTACK_COMBO_04_MONTAGE.Object;
	}



	// JumpAttack
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_ATTACK_COMBO_01_MONTAGE(TEXT("/Game/Frank_RPG_Warrior/Animations/DS_Mannequin_RM/DS_Frank_RPG_Warrior_Attack04_Montage.DS_Frank_RPG_Warrior_Attack04_Montage"));
	if (JUMP_ATTACK_COMBO_01_MONTAGE.Succeeded())
	{
		JumpAttackMontage_Combo01 = JUMP_ATTACK_COMBO_01_MONTAGE.Object;
	}





	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_REACTION_FRONT(TEXT("/Game/JogMoveAnim/DS_HitReaction_Front.DS_HitReaction_Front"));
	if (HIT_REACTION_FRONT.Succeeded())
	{
		HitReactionFront = HIT_REACTION_FRONT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_REACTION_BACK(TEXT("/Game/JogMoveAnim/DS_HitReaction_Back.DS_HitReaction_Back"));
	if (HIT_REACTION_BACK.Succeeded())
	{
		HitReactionBack = HIT_REACTION_BACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_REACTION_LEFT(TEXT("/Game/JogMoveAnim/DS_HitReaction_Left.DS_HitReaction_Left"));
	if (HIT_REACTION_LEFT.Succeeded())
	{
		HitReactionLeft = HIT_REACTION_LEFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_REACTION_RIGHT(TEXT("/Game/JogMoveAnim/DS_HitReaction_Right.DS_HitReaction_Right"));
	if (HIT_REACTION_RIGHT.Succeeded())
	{
		HitReactionRight = HIT_REACTION_RIGHT.Object;
	}

	//Rolling
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACKWARD(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollBackward_Root_Montage.DS_RollBackward_Root_Montage"));
	if (ROLL_BACKWARD.Succeeded())
	{
		RollBackward = ROLL_BACKWARD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACKWARD_LEFT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollBackwardLeft_Root_Montage.DS_RollBackwardLeft_Root_Montage"));
	if (ROLL_BACKWARD_LEFT.Succeeded())
	{
		RollBackwardLeft = ROLL_BACKWARD_LEFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACKWARD_RIGHT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollBackwardRight_Root_Montage.DS_RollBackwardRight_Root_Montage"));
	if (ROLL_BACKWARD_RIGHT.Succeeded())
	{
		RollBackwardRight = ROLL_BACKWARD_RIGHT.Object;
	}


	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWAD(TEXT("/Game/Frank_RPG_Warrior/Animations/DS_Mannequin_RM/DS_Frank_RPG_Warrior_Combo03_All_Montage.DS_Frank_RPG_Warrior_Combo03_All_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWAD(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollForward_Root_Montage.DS_RollForward_Root_Montage"));
	if (ROLL_FORWAD.Succeeded())
	{
		RollForward = ROLL_FORWAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWADLEFT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollForwardLeft_Root_Montage.DS_RollForwardLeft_Root_Montage"));
	if (ROLL_FORWADLEFT.Succeeded())
	{
		RollForwardLeft = ROLL_FORWADLEFT.Object;
	}



	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWADRIGHT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollForwardRight_Root_Montage.DS_RollForwardRight_Root_Montage"));
	if (ROLL_FORWADRIGHT.Succeeded())
	{
		RollForwardRight = ROLL_FORWADRIGHT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_LEFT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollLeft_Root_Montage.DS_RollLeft_Root_Montage"));
	if (ROLL_LEFT.Succeeded())
	{
		RollLeft = ROLL_LEFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_RIGHT(TEXT("/Game/RollsAndDodges/Animations/RootMotion/DS_RollRight_Root_Montage.DS_RollRight_Root_Montage"));
	if (ROLL_RIGHT.Succeeded())
	{
		RollRight = ROLL_RIGHT.Object;
	}



}



void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ƽ���� �� ��������
	APawn* pPawn = TryGetPawnOwner();
	
	// ������Ʈ�� ��밡���� �������� Ȯ�����ִ� �Լ� ��밡���ϸ� True;
	if (!::IsValid(pPawn)) return; // ���Ұ����ϸ� ����

	if (!IsDead)
	{
		fCurrentPawnSpeed = pPawn->GetVelocity().Size();

		/*if (pPawn->IsPlayerControlled()) DSLOG(Warning, TEXT("fCurrentPawnSpeed : %f"), fCurrentPawnSpeed);*/

		ACharacter* pCharacter = Cast<ACharacter>(pPawn);

		// �ٶ󺸴� ����� �����̴� ������ ���� ������ ��ȯ����
		fDirection = CalculateDirection(pCharacter->GetVelocity(), pCharacter->GetActorRotation());

	
		
		if (pCharacter->IsPlayerControlled())
		{
			//DSLOG(Warning, TEXT("%f, %f, %f"), pCharacter->GetActorLocation().X, pCharacter->GetActorLocation().Y, pCharacter->GetActorLocation().Z);
		}

		if (pCharacter->IsPlayerControlled())
		{
			//DSLOG(Warning, TEXT("%f"), fDirection);
			//DSLOG(Warning, TEXT("%f"), fCurrentPawnSpeed);
		}

		// ���߿� ���ִ��� Ȯ��
		if (pCharacter)
		{
			bIsInAir = pCharacter->GetMovementComponent()->IsFalling();
		}
	}
}


void UDSAnimInstance::PlayAttackMontage()
{
	DSCHECK(!IsDead);

	switch (m_nAttackComboType)
	{
	case 0:
	{	
		if (!Montage_IsPlaying(AttackMontage_old))
		{
			Montage_Play(AttackMontage_old, 1.0f);
			break;
		}
	}

	case 1:
	{
		if (!Montage_IsPlaying(AttackMontage_Combo01))
		{
			Montage_Play(AttackMontage_Combo01, 1.0f);
			break;
		}
	}

	case 2:
	{
		if (!Montage_IsPlaying(AttackMontage_Combo02))
		{
			Montage_Play(AttackMontage_Combo02, 1.0f);
			break;
		}
	}

	case 3:
	{
		if (!Montage_IsPlaying(AttackMontage_Combo03))
		{
			Montage_Play(AttackMontage_Combo03, 1.0f);
			break;
		}
	}

	case 4:
	{
		if (!Montage_IsPlaying(AttackMontage_Combo04))
		{
			Montage_Play(AttackMontage_Combo04, 1.0f);
			break;
		}
	}

	}
}

void UDSAnimInstance::PlayJumpAttackMontage()
{
	DSCHECK(!IsDead);

	if (!Montage_IsPlaying(JumpAttackMontage_Combo01))
	{
		Montage_Play(JumpAttackMontage_Combo01, 1.0f);
		
	}

}




void UDSAnimInstance::PlayHitReactionFront()
{
	DSCHECK(!IsDead);
	if (!Montage_IsPlaying(HitReactionFront))
	{
		Montage_Play(HitReactionFront, 1.0f);
	}
}

void UDSAnimInstance::PlayHitReactionBack()
{
	DSCHECK(!IsDead);
	if (!Montage_IsPlaying(HitReactionBack))
	{
		Montage_Play(HitReactionBack, 1.0f);
	}
}

void UDSAnimInstance::PlayHitReactionLeft()
{
	DSCHECK(!IsDead);
	if (!Montage_IsPlaying(HitReactionLeft))
	{
		Montage_Play(HitReactionLeft, 1.0f);
	}
}

void UDSAnimInstance::PlayHitReactionRight()
{
	DSCHECK(!IsDead);
	if (!Montage_IsPlaying(HitReactionRight))
	{
		Montage_Play(HitReactionRight, 1.0f);
	}
}

void UDSAnimInstance::PlayRollMontage()
{
	APawn* pPawn = TryGetPawnOwner();

	if (!::IsValid(pPawn)) return; // ���Ұ����ϸ� ����

	ADSCharacter* pCharacter = Cast<ADSCharacter>(pPawn);


	int nMoveForward = pCharacter->InputComponent->GetAxisValue(TEXT("MoveForward"));
	int nMoveRight = pCharacter->InputComponent->GetAxisValue(TEXT("MoveRight"));

	// �븻 ����϶� ������ ������ �����ϱ� ���ؼ�
	//FVector vecCurrentDir;
	//vecCurrentDir.X = nMoveForward;
	//vecCurrentDir.Y = nMoveRight;
	//vecCurrentDir.Z = 0;
	//
	//FRotator InputRotator;
	//InputRotator.Pitch = 0;
	//InputRotator.Roll = 0;
	//InputRotator.Yaw = pCharacter->GetControlRotation().Yaw;
	//
	//if (vecCurrentDir.Size() > 0)
	//{
	//	pCharacter->SetActorRotation(InputRotator);
	//}

	DSCHECK(!IsDead);
	
	if (IsRolling() || pCharacter->IsCharacterAttacking())
	{
		return;
	}

	// �븻�̶�� ���鱸������� �ȵ���
	if (pCharacter->GetCurrentControlMode() == EControlMode::eNomal)
	{
		Montage_Play(RollForward, 1.0f);
		return;
	}

	// ���� �밢������� Ȯ���ؾ���.
	if (nMoveForward == -1 && nMoveRight == -1)
	{
		Montage_Play(RollBackwardLeft, 1.0f);
		return;
	}
	else if (nMoveForward == -1 && nMoveRight == 1)
	{
		Montage_Play(RollBackwardRight, 1.0f);
		return;
	}
	else if (nMoveForward == 1 && nMoveRight == -1)
	{
		Montage_Play(RollForwardLeft, 1.0f);
		return;
	}
	else if (nMoveForward == 1 && nMoveRight == 1)
	{
		Montage_Play(RollForwardRight, 1.0f);
		return;
	}
	
	// �ܹ���
	else if (nMoveForward == -1)
	{
		Montage_Play(RollBackward, 1.0f);
		return;
	}
	else if (nMoveForward == 1)
	{
		Montage_Play(RollForward, 1.0f);
		return;
	}
	else if (nMoveRight == -1)
	{
		Montage_Play(RollLeft, 1.0f);
		return;
	}
	else if (nMoveRight == 1)
	{
		Montage_Play(RollRight, 1.0f);
		return;
	}

}


void UDSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	DSCHECK(!IsDead);
	//DSCHECK(Montage_IsPlaying(AttackMontage))

	// ���� ��Ÿ�� �������� �Ѿ��
	// �Ű����� (�Ѿ ���� ���ڿ�, �ش� ��Ÿ��)

	switch (m_nAttackComboType)
	{

	case 0:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_old);
		break;
	case 1:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_Combo01);
		break;
	case 2:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_Combo02);
		break;
	case 3:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_Combo03);
		break;
	case 4:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_Combo04);
		break;

	default:
		break;
	}

	
}

void UDSAnimInstance::AnimNotify_AttackHit()
{
	//DSLOG_S(Warning);

	OnAttackHit.Broadcast();
}

void UDSAnimInstance::AnimNotify_NextAttackCheck()
{
	//DSLOG_S(Warning);

	OnNextAttackCheck.Broadcast();
}

void UDSAnimInstance::AnimNotify_LastAttack()
{
	//DSLOG_S(Warning);

	OnLastAttack.Broadcast();
}

// Attack1~3 ������ �����̸��� �´� ���� ���� ���� �������ִ� �Լ�
FName UDSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	DSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	// Attack1~3 ������ �����̸��� �´� ���� ���� ���� �������ִ� �Լ�
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UDSAnimInstance::AnimNotify_RightPlant()
{
	//DSLOG_S(Warning);
}

void UDSAnimInstance::AnimNotify_LeftPlant()
{
	//DSLOG_S(Warning);
}

void UDSAnimInstance::SetHitDirection(float fValue)
{
	fHitDirection = fValue;
}

void UDSAnimInstance::SetDeadAnim(bool bValue)
{
	IsDead = bValue;
}

bool UDSAnimInstance::FunctionIsDead()
{
	return IsDead;
}

void UDSAnimInstance::SetRunInputCheck(bool bValue)
{
	bRunInputCheck = bValue;
}

bool UDSAnimInstance::GetRunInputCheck()
{
	return bRunInputCheck;
}


void UDSAnimInstance::SetGuardInputCheck(bool bValue)
{
	bGuardInputCheck = bValue;
}

bool UDSAnimInstance::GetGuardInputCheck()
{
	return bGuardInputCheck;
}



bool UDSAnimInstance::IsRolling()
{
	bool bResult = false;

	if (Montage_IsPlaying(RollBackwardLeft) || Montage_IsPlaying(RollBackwardRight) || Montage_IsPlaying(RollForwardLeft) || Montage_IsPlaying(RollForwardRight) || Montage_IsPlaying(RollBackward) || Montage_IsPlaying(RollForward) || Montage_IsPlaying(RollLeft) || Montage_IsPlaying(RollRight))
	{
		bResult = true;
	}

	return bResult;
}

bool UDSAnimInstance::AutoAttackCombo(int nAttackComboType)
{
	bool bResult = false;

	/*PlayAttackMontage(nAttackComboType);*/

	// ���� ī��Ʈ������ ����



	return bResult;
}

void UDSAnimInstance::SetAttackComboType(int nAttackComboType)
{
	APawn* pPawn = TryGetPawnOwner();
	ADSCharacter* pDSCharacter = Cast<ADSCharacter>(pPawn);

	m_nAttackComboType = pDSCharacter->GetAttackComboType();
}

UAnimMontage* UDSAnimInstance::GetRollBackward()
{
	return RollBackward;
}