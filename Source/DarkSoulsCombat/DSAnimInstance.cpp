// Fill out your copyright notice in the Description page of Project Settings.

#include "DSAnimInstance.h"

UDSAnimInstance::UDSAnimInstance()
{
	fCurrentPawnSpeed = 0.0f;
	fDirection = 0.0f;
	bIsInAir = false;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AnimStarterPack/DSCharactor_AttackMontage.DSCharactor_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
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
}

void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 틱에서 폰 가져오기
	APawn* pPawn = TryGetPawnOwner();

	// 오브젝트가 사용가능한 상태인지 확인해주는 함수 사용가능하면 True;
	if (!::IsValid(pPawn)) return; // 사용불가능하면 리턴


	if (!IsDead)
	{
		fCurrentPawnSpeed = pPawn->GetVelocity().Size();
		ACharacter* pCharacter = Cast<ACharacter>(pPawn);

		// 바라보는 방향과 움직이는 방향의 사이 각도를 반환해줌
		fDirection = CalculateDirection(pCharacter->GetVelocity(), pCharacter->GetActorRotation());
		if (pCharacter->IsPlayerControlled())
		{
			DSLOG(Warning, TEXT("%f"), fDirection);
			DSLOG(Warning, TEXT("%f"), fCurrentPawnSpeed);
		}

		// 공중에 떠있는지 확인
		if (pCharacter)
		{
			bIsInAir = pCharacter->GetMovementComponent()->IsFalling();
		}
	}
}

void UDSAnimInstance::PlayAttackMontage()
{
	DSCHECK(!IsDead);
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
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

void UDSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	DSCHECK(!IsDead);
	//DSCHECK(Montage_IsPlaying(AttackMontage))

	// 다음 몽타주 섹션으로 넘어가기
	// 매개변수 (넘어갈 섹션 문자열, 해당 몽타주)
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

void UDSAnimInstance::AnimNotify_LastAttack()
{
	DSLOG_S(Warning);

	OnLastAttack.Broadcast();
}

// Attack1~3 까지의 섹션이름에 맞는 섹션 네임 만들어서 리턴해주는 함수
FName UDSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	DSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	// Attack1~3 까지의 섹션이름에 맞는 섹션 네임 만들어서 리턴해주는 함수
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UDSAnimInstance::AnimNotify_RightPlant()
{
	DSLOG_S(Warning);
}

void UDSAnimInstance::AnimNotify_LeftPlant()
{
	DSLOG_S(Warning);
}

void UDSAnimInstance::SetHitDirection(float fValue)
{
	fHitDirection = fValue;
}

void UDSAnimInstance::SetDeadAnim(bool bValue)
{
	IsDead = bValue;
}
