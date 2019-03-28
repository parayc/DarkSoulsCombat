// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "DSCharacterStatComponent.h"
#include "DSAIController.h"
#include "DSAnimInstance.h"
#include "DSCharacterWidget.h"


// Sets default values
ADSCharacter::ADSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastAttacker = nullptr;

	AIControllerClass = ADSAIController::StaticClass();
	// �ڵ����� �÷���ư� ������ ĳ���͸� �����ϰ�� AICotroller�� �����ϵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// �ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DSCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	TargetUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("TARGETUI"));
	HPBarUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARUI"));
	// ĳ���� ����
	CharacterStat = CreateDefaultSubobject<UDSCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	TargetUI->SetupAttachment(GetMesh());
	HPBarUI->SetupAttachment(GetMesh());
	
	// TargetLockOn UI�� ��� ĳ���͸��� ����� �� ������ �⺻���� �س��´�.
	TargetUI->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	TargetUI->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_TARGETLOCKON(TEXT("/Game/Enemy/UI/UI_TargetLockOn.UI_TargetLockOn_C"));
	if (UI_TARGETLOCKON.Succeeded())
	{
		TargetUI->SetWidgetClass(UI_TARGETLOCKON.Class);
		TargetUI->SetDrawSize(FVector2D(35.0f, 35.0f));
	}

	TargetUI->SetHiddenInGame(true);

	HPBarUI->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	HPBarUI->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBarUI->SetWidgetClass(UI_HPBAR.Class);
		HPBarUI->SetDrawSize(FVector2D(150.0f, 50.0f));
	}




	// ����Ʈ���� 3���� ��ǥ��� �𸮾� ���� 3���� ��ǥ�谡 �޶� �����ַ��� -90,
	// �𸮾������� ������ ������ġ�� ������ ���߾������� ĳ���� ������ �߹ٴں��� �׷��� ���� ���� -
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	GetCharacterMovement()->JumpZVelocity = 600.f;



	// ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	// �޽��� �ִϸ��̼��� �ִ� �������Ʈ�� ������ ��
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// �ִϸ��̼� �������Ʈ ���� 
	static ConstructorHelpers::FClassFinder<UAnimInstance> StartPack_Anim(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint.UE4ASP_HeroTPP_AnimBlueprint_C"));
	if (StartPack_Anim.Succeeded())
	{ 
		GetMesh()->SetAnimInstanceClass(StartPack_Anim.Class);
	}

	// ���� �Ҹ�
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_AttackSoundCue(TEXT("/Game/Weapon_Pack/SwordSound/A_Sword_Swing_Cue.A_Sword_Swing_Cue"));
	if (SC_AttackSoundCue.Succeeded())
	{
		AttackSoundCue = SC_AttackSoundCue.Object;
		AttackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComponent"));
		AttackAudioComponent->SetupAttachment(RootComponent);
	}


	// Hit Grunt(�δ�) �Ҹ�
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_HitGruntSoundCue(TEXT("/Game/SoundSoruce/ManGrunt/SC_ManGrunt.SC_ManGrunt"));
	if (SC_HitGruntSoundCue.Succeeded())
	{
		HitGruntSoundCue = SC_HitGruntSoundCue.Object;
		HitGruntAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitGruntAudioComponent"));
		HitGruntAudioComponent->SetupAttachment(RootComponent);
	}

	// �ȱ� �Ҹ�
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_FootStepSoundCue(TEXT("/Game/AnimStarterPack/Sound/A_Character_Step_Cue.A_Character_Step_Cue"));
	if (SC_FootStepSoundCue.Succeeded())
	{
		FootStepSoundCue = SC_FootStepSoundCue.Object;
		FootStepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepAudioComponent"));
		FootStepAudioComponent->SetupAttachment(RootComponent);
	}
	




	// �޺������� ���� ����
	IsAttacking = false; // ���� ���� ��Ÿ�� ���������� �ľ�
	MaxCombo = 4; // �޺����� �ܰ� �ִ�ġ
	AttackEndComboState(); // ������ ������ �޺����� ���õ� ���� �ʱ�ȭ���ַ���

	IsParrying = false;

	// ���� �޺� Ÿ�� ���� �⺻ 1	
	m_bGuard = false;

	m_bPressedMouseLeft = false;
	m_bPressedMouseRight = false;

	m_bPressedGuard = false;
	m_bPressedRun = false;
	m_bRolling = false;

	m_fMouseLRClickCheckTime = 0;

	m_bCanParrying = false;

	m_bStun = false;

	SetControlMode(eControlMode);
}

void ADSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DSAnim = Cast<UDSAnimInstance>(GetMesh()->GetAnimInstance());

	SetAttackComboType(1);

	DSAnim->OnAttackHit.AddUObject(this, &ADSCharacter::AttackCheck);

	DSAnim->OnParryingCheck.AddUObject(this, &ADSCharacter::ParryingCheck);

	DSAnim->OnMontageEnded.AddDynamic(this, &ADSCharacter::OnAttackMontageEnded);

	DSAnim->OnMontageEnded.AddDynamic(this, &ADSCharacter::OnRollingMontageEnded);

	DSAnim->OnMontageEnded.AddDynamic(this, &ADSCharacter::OnParryingMontageEnded);

	OnMouseLRClickCheck.AddLambda([=]() -> void {
		OnParrying();
	});


	// �޺����� ���� ���
	DSAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		// �޺� ���� �Է��� ���̸� 
		if (IsComboInputOn)
		{
			// �޺� ������ �˸��� �Լ�
			AttackStartComboState();
			// ���� ������ �����ش�
			// ���� ���� �������� AttackStartComboState �Լ� ���ļ� ���� �������� �Ѿ ���� �ѹ�
			DSAnim->JumpToAttackMontageSection(CurrentCombo);
/*
			if (AttackAudioComponent && AttackSoundCue)
			{
				AttackAudioComponent->Play(0.f);
			}*/

		}

	});

	DSAnim->OnLastAttack.AddLambda([this]() -> void {
		CurWeapon->PlayLastAttackEffect();
	});

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		DSAnim->SetDeadAnim(true);

		//GetCharacterMovement()->DisableMovement();
		SetActorEnableCollision(false);
		TargetUI->SetHiddenInGame(true);
		LastAttacker->Target();
	});

	DSAnim->OnParryingTimeStart.AddLambda([=]() -> void {
		m_bCanParrying = true;
		DSLOG(Warning, TEXT("m_bCanParrying = true"));
	});

	DSAnim->OnParryingTimeEnd.AddLambda([=]() -> void {
		m_bCanParrying = false;
		DSLOG(Warning, TEXT("m_bCanParrying = false"));
	});



}

// Called when the game starts or when spawned
void ADSCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	FName Socket_RightHand_Weapon(TEXT("Socket_RightHand_Weapon"));
	CurWeapon = GetWorld()->SpawnActor<ADSWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket_RightHand_Weapon);
	}

	



	FName Socket_LeftHand_Shield(TEXT("Socket_LeftHand_Shield"));
	CurShield = GetWorld()->SpawnActor<ADSShield>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurShield)
	{
		CurShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket_LeftHand_Shield);
		CurShield->SetActorEnableCollision(false);
	}






	if (AttackAudioComponent && AttackSoundCue)
	{
		AttackAudioComponent->SetSound(Cast<USoundBase>(AttackSoundCue));
	}

	if (FootStepAudioComponent && FootStepSoundCue)
	{
		FootStepAudioComponent->SetSound(Cast<USoundBase>(FootStepSoundCue));
	}

	if (HitGruntSoundCue && HitGruntAudioComponent)
	{
		HitGruntAudioComponent->SetSound(Cast<USoundBase>(HitGruntSoundCue));
	}

	// �̰� �� BeginPlay()���� �ؾ�����??
	auto CharacterWidget = Cast<UDSCharacterWidget>(HPBarUI->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}


// Called every frame
void ADSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	//if (!IsPlayerControlled())
	//{
	//	if (CameraTarget != nullptr)
	//	{
	//		bUseControllerRotationYaw = false;
	//		GetCharacterMovement()->bOrientRotationToMovement = false;

	//		FVector TargetVect = CameraTarget->GetActorLocation() - GetActorLocation();
	//		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
	//		FRotator CurrentRot = GetActorRotation();
	//		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 100.f);

	//		SetActorRotation(NewRot);
	//	}
	//}

	//if (m_bPressedMouseLeft || m_bPressedMouseRight)
	//{
	//	m_fMouseLRClickCheckTime += DeltaTime;

	//	if (m_fMouseLRClickCheckTime >= 0.15f)
	//	{
	//		m_bPressedMouseLeft = false;
	//		m_bPressedMouseRight = false;
	//		m_fMouseLRClickCheckTime = 0;
	//	}
	//}

	//if (m_bPressedMouseLeft && m_bPressedMouseRight)
	//{
	//	OnMouseLRClickCheck.Broadcast();
	//}

/*
	if (m_bPressedMouseLeft && m_bPressedGuard)
	{
		OnMouseLRClickCheck.Broadcast();
	}*/


	RadialDetection(DeltaTime);


}

// Called to bind functionality to input
void ADSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADSCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADSCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADSCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADSCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("ModeChange"), EInputEvent::IE_Pressed, this, &ADSCharacter::ModeChange);
	PlayerInputComponent->BindAction(TEXT("Target"), EInputEvent::IE_Pressed, this, &ADSCharacter::Target);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ADSCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ADSCharacter::StartRun);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &ADSCharacter::StopRun);
	PlayerInputComponent->BindAction(TEXT("Guard"), EInputEvent::IE_Pressed, this, &ADSCharacter::StartGuard);
	PlayerInputComponent->BindAction(TEXT("Guard"), EInputEvent::IE_Released, this, &ADSCharacter::StopGuard);
	PlayerInputComponent->BindAction(TEXT("ForwardRoll"), EInputEvent::IE_Pressed, this, &ADSCharacter::ForwardRoll);
	PlayerInputComponent->BindAction(TEXT("MouseLeftClick"), EInputEvent::IE_Released, this, &ADSCharacter::MouseLeftClick);
	PlayerInputComponent->BindAction(TEXT("MouseRightClick"), EInputEvent::IE_Pressed, this, &ADSCharacter::MouseRightClick);
	PlayerInputComponent->BindAction(TEXT("Parrying"), EInputEvent::IE_Pressed, this, &ADSCharacter::Parrying);

}

void ADSCharacter::UpDown(float NewAxisValue)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsStun()) return;

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	// GetActorForwardVector() ������ �������� �� ��������
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
}


void ADSCharacter::LeftRight(float NewAxisValue)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsStun()) return;



	// GetActorRightVector() ������ ������ �� ��������
	//AddMovementInput(GetActorRightVector(), NewAxisValue);
	
	const FRotator Rotation = CameraTarget == nullptr ? GetControlRotation() : (CameraTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, NewAxisValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ADSCharacter::LookUp(float NewAxisValue)
{
	if (IsStun()) return;

	AddControllerPitchInput(NewAxisValue);
}

void ADSCharacter::Turn(float NewAxisValue)
{
	if (IsStun()) return;

	AddControllerYawInput(NewAxisValue);
}


void ADSCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DSCHECK(IsAttacking);
	DSCHECK(CurrentCombo > 0);
	/*
	if (IsAttacking == false || CurrentCombo > 0)
	{
		return;
	}*/
	IsAttacking = false;
	AttackEndComboState();

	OnAttackEnd.Broadcast();
}

void ADSCharacter::OnRollingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DSCHECK(m_bRolling);
	
	m_bRolling = false;

	if (m_bPressedGuard)
	{
		StartGuard();
	}
}




void ADSCharacter::ModeChange()
{
	if (eControlMode == EControlMode::eNomal)
	{
		eControlMode = EControlMode::eDarkSouls;
	}
	else if (eControlMode == EControlMode::eDarkSouls)
	{
		eControlMode = EControlMode::eNomal;
	}

	SetControlMode(eControlMode);
}

void ADSCharacter::SetControlMode(EControlMode eMode)
{

	// ���� ����
	SpringArm->TargetArmLength = 450.f;

	// ����, ȸ�� ����
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

	// ���� ī�޶� �����̼��� �����Ұ���
	SpringArm->bUsePawnControlRotation = true;

	// �θ�(ĸ����) X,Y,Z���� ��ӹ޾Ƽ� �״�� ������
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// ī�޶��� �þ� �߰��� ���ع��� ������ �� �ձ��� �þ� �������
	SpringArm->bDoCollisionTest = true;	
	
	// ������ ��� �޸� ī�޶� ������ ����� �ѹ��� ������ ��������� �ϱ� ���� �ɼ�
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->CameraLagSpeed = 1.f;
	SpringArm->CameraRotationLagSpeed = 1.f;
	SpringArm->CameraLagMaxDistance = 100.f;

	switch (eMode)
	{

	case EControlMode::eNomal:
	{
		eControlMode = EControlMode::eNomal;

		// ��Ʈ�� ȸ���� Yaw��� Pawn�� Yaw�� ������������ Ȯ���ϴ� ��� true�� ����
		// ������� �̰� false�� �����ϸ� ī�޶� �¿�� �������� ī�޶� ������ �̵��ϰ�
		// ĳ���ʹ� �� �������� ���ư��� ����.
		bUseControllerRotationYaw = false;

		// ĳ���Ͱ� �������� �������� ĳ���͸� �ڵ����� �����ٰ���
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

		break;
	}

	case EControlMode::eDarkSouls:
	{
		eControlMode = EControlMode::eDarkSouls;

		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

		break;
	}

	default:
		break;
	}
}

void ADSCharacter::RadialDetection(float DeltaTime)
{
	//float fDetectRadius = 600.0f;

	//// �̰͵� ��� ���� �ִ°� �´�.
	//// �ݰ� 6���Ϳ� ��� ������Ʈ Ž��
	//TArray<FOverlapResult> arrOverlapResults;
	//FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	//bool bResult = GetWorld()->OverlapMultiByChannel(
	//	arrOverlapResults,
	//	GetActorLocation(),
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	FCollisionShape::MakeSphere(fDetectRadius),
	//	CollisionQueryParam
	//);

	//// Ž���� ������Ʈ�� ĳ�������� �Ǻ��ϰ�
	//// ���� ǥ�� �׸�, Ž�� ������Ʈ���� �� �׸���
	//
	//
	//float ClosestDotToCenter = 0.f;

	//if (bResult)
	//{
	//	if (arrOverlapResults.Num() >= 0)
	//	{
	//		for (auto OverlapResult : arrOverlapResults)
	//		{
	//			ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
	//			if (DSCharacter)
	//			{
	//				DrawDebugSphere(GetWorld(), GetActorLocation(), fDetectRadius, 16, FColor::Green, false, 0.2f);
	//				DrawDebugPoint(GetWorld(), DSCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
	//				DrawDebugLine(GetWorld(), this->GetActorLocation(), DSCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);

	//				float Dot = FVector::DotProduct(GetActorForwardVector(), (DSCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal());

	//				if (Dot > ClosestDotToCenter)
	//				{
	//					ClosestDotToCenter = Dot;
	//					CameraTarget = DSCharacter;
	//				}
	//				
	//				//ABLOG(Warning, TEXT("%f"), fResult);
	//			}
	//		}
	//	}
	//	return;
	//}


	// �̰��� ƽ���� ��� ���ư����ϴ� ����� �¾�
	// ī�޶� Ÿ���� �ٶ󺸵��� ����� �κ�

	if (IsPlayerControlled())
	{
		if (CameraTarget != nullptr)
		{
			FVector TargetVect = CameraTarget->GetActorLocation() - GetActorLocation();
			FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
			FRotator CurrentRot = GetControlRotation();
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 10.f);

			// ��Ʈ�ѷ���(���콺 ����)���� �� ĳ������ �ü������� ��ġ�ϴ� �ɼ��� �ѳ��ұ⿡ ��Ʈ�ѷ��� ������ �����ָ� ĳ���Ͱ� �ڵ����� �װ��� �ٶ󺻴�.
			GetController()->SetControlRotation(NewRot);

		}
	}
	//else if (CameraTarget != nullptr)
	//{
	//	CameraTarget = nullptr;
	//}

	//DrawDebugSphere(GetWorld(), GetActorLocation(), fDetectRadius, 16, FColor::Red, false, 0.2f);
}

void ADSCharacter::Target()
{
	GetTarget();

	if (!IsPlayerControlled()) return;

	if (CameraTarget != nullptr)
	{
		SetControlMode(EControlMode::eDarkSouls);
	}
	else
	{
		SetControlMode(EControlMode::eNomal);
	}
}

void ADSCharacter::GetTarget()
{
	ADSCharacter* PrevCameraTarget = nullptr;

	if (CameraTarget != nullptr)
	{
		PrevCameraTarget = CameraTarget;
	}

	// TabŰ�� ������ �������� ���� ����� Ÿ���� CameraTarget���� �������ش�.
	float fDetectRadius = 600.0f;

	TArray<FOverlapResult> arrOverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		arrOverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(fDetectRadius),
		CollisionQueryParam
	);

	float ClosestDotToCenter = 0.f;

	// Ž���� ����� �����Ѵٸ�
	if (bResult)
	{
		// Ž���� ����� ����ִ� �迭�� 1�� �̻��� ���� ����ִٸ�
		if (arrOverlapResults.Num() >= 0)
		{
			// �迭�� ����ŭ �ݺ�
			for (auto OverlapResult : arrOverlapResults)
			{
				ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
				if (DSCharacter)
				{
					// Ž���� ������ ��ġ�� �÷��̾� ĳ������ ��ġ�� ���� ���� ���Ͱ���
					// ī�޶��� ���� ���� ������ ������ ���Ͽ�
					// ��ġ�� ���� ���ٸ� ī�޶� ���� ���⿡ ���� ������ ������Ʈ �Ǻ�
					float Dot = FVector::DotProduct(Camera->GetForwardVector(), (DSCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal());

					if (Dot > ClosestDotToCenter)
					{
						ClosestDotToCenter = Dot;
						CameraTarget = DSCharacter;
						
					}
				}
			}

			// ���� Ÿ�ٰ� ���� ���õ� Ÿ���� �ٸ��ٸ� ���� Ÿ���� TargetLockOn UI�� �������� ����
			if (PrevCameraTarget != CameraTarget && PrevCameraTarget != nullptr)
			{
				PrevCameraTarget->TargetUI->SetHiddenInGame(true);
			}

			// Ÿ���� �������ٸ� Ÿ���� TargetLockOn UI�� oN
			if (CameraTarget != nullptr)
			{
				if (IsPlayerControlled())
				{
					CameraTarget->TargetUI->SetHiddenInGame(false);
				}
			}
		}
	}
	else
	{
		// �������� ���õ� Ÿ���� ���ٸ�
		CameraTarget = nullptr;

		// ���� ���� Ÿ���� �־��� ��� ���� Ÿ���� TargetLockOn UI�� �������� ����
		if (PrevCameraTarget != CameraTarget && PrevCameraTarget != nullptr)
		{
			PrevCameraTarget->TargetUI->SetHiddenInGame(true);
		}
	}
}

/*
���� �ý��� ����
DSCharacter��  NextAttackCheck ��Ƽ���̸� ���Ǻ��� �����
ù��° ���ݽÿ��� CanNextCombo�� true �����Ͽ� �޺��� ������ ���·� �����
CurrentCombo�� +1 �����ش�
�� ���� ���� ��Ÿ�ָ� �����Ű��
���ݽ� IsAttacking������ true�� ����

ù��° ���� �����߿� Attack�Լ��� ȣ����� ��
���� ������ �������ϰ�� CurrentCombo�� 1~3 ���̿� �ִ����� �Ǻ��ϸ�
�̹� ù��° ������ ���� �Ǿ��� ���⿡ ������ 1 �̻��� �� �ۿ� ����.
�Ǻ������ ���ϰ��
CanNextCombo�� true�� ��� IsComboInputOn�� true�� �����Ͽ�
NextAttackCheck ��Ƽ���̰� �߻��Ͽ��� �� IsComboInputOn�� Ȯ���Ͽ�
true�ϰ�� ���� �������� �Ѿ �� �ְ� ���ش�

����
NextAttackCheck ��Ƽ���� �߻�����
�޺� ������ �ϰ����ϴ� ��ȣ�� �߻��Ͽ������ ��Ƽ���� �̺�Ʈ �Լ�����
���� �������� �Ѿ�� �޺��� �̾���




*/
// �Ϲݰ��� �Լ�
void ADSCharacter::Attack()
{
	if (m_bGuard) return;
	if (IsStun()) return;


	//m_bPressedMouseLeft = true;

	//if (m_bPressedMouseLeft && m_bPressedMouseRight)
	//{
	//	DSCHECK(IsParrying);
	//	Parrying();
	//}
	//else
	//{
		// ���߰���
		//if (GetMovementComponent()->IsFalling())
		//{
		//	if (DSAnim->IsRolling() == true)
		//	{
		//		return;
		//	}

		//	DSAnim->PlayJumpAttackMontage();

		//	//if (AttackAudioComponent && AttackSoundCue)
		//	//{
		//	//	AttackAudioComponent->Play(0.f);
		//	//}
		//}
		//else
		//{
		if (DSAnim->IsRolling() == true)
		{
			return;
		}

		// �������̶�� ùȸ ������������ ���� ��Ž 2��° �޺����� Ž
		if (IsAttacking)
		{
			DSCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo))

				if (CanNextCombo)
				{
					IsComboInputOn = true;

				}
		}
		else
		{
			DSCHECK(CurrentCombo == 0);
			AttackStartComboState();
			DSAnim->PlayAttackMontage();
			//DSAnim->JumpToAttackMontageSection(CurrentCombo);
			IsAttacking = true;

			/*		if (AttackAudioComponent && AttackSoundCue)
					{
						AttackAudioComponent->Play(0.f);
					}*/
		}
		//}
	//}

	//m_bPressedMouseLeft = false;
}

void ADSCharacter::JumpAttack()
{
	if (DSAnim->IsRolling() == true)
	{
		return;
	}

	DSAnim->PlayJumpAttackMontage();
	AttackStartComboState();
	IsAttacking = true;
/*
	if (AttackAudioComponent && AttackSoundCue)
	{
		AttackAudioComponent->Play(0.f);
	}*/
}

void ADSCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f, // ĳ���� ���� 2���ͱ��� �浹 �߻����� Ȯ��
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(30.0f),
		Params
		);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ADSShield* DSShield = Cast<ADSShield>(HitResult.GetActor());


			if (DSShield)
			{

				// ���а� ������ٴ°Ÿ� ������ �ݸ����� ���ִٴ°��̸� �ᱹ �װ��� ���и� ����ִ� ĳ���Ͱ� ���� �� �����̶�� �Ҹ�
				// �׷��ٸ� ���⼭ ������ ������������ �۾��� �����ϸ� ��
				// ������ ���п� ���θ������� �ؾ��ϴ� �۾�
				// ���п� ������ ���� ť ����
				// ���� ť ��ü�� �ϳ� ������ ������?
				// ���䰡 �Ҹ��� �����ұ�?
				// 
				if (DSShield != CurShield)
				{
					ADSCharacter* GuardCharacter = Cast<ADSCharacter>(DSShield->GetAttachParentActor());

					DSLOG(Warning, TEXT("Shield Hit!!!"));
					DSShield->PlayHitSound();

					GuardCharacter->DSAnim->PlayShieldBlockMontage();
					GuardCharacter->PlayKnockBack(40.f);

				}
			}


			DSLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			
			FDamageEvent DamageEvent;
			// �������� ���ϴ� ������ �����ڴ� ���̾ƴ϶� �÷��̾� ��Ʈ���̴�!!
			// �ĸ����� �տ������� ��������, ������ ����, ���� ����� ���� ��.��.�ڴ� �÷��̾� ��Ʈ�ѷ�, ����� ������ ��(ĳ����)
			// �÷��̾� ��Ʈ�ѷ���� �����ڰ� ���̶�� ���ص����� �̿��ؼ� ������ ��
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			
			CurWeapon->PlayHitEffect();
		}
	}
}

float ADSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CharacterStat->SetDamage(FinalDamage);

	HitGruntAudioComponent->Play(0.f);

	// ���������� ������ ����� ������ Ȯ���ϱ� ���ؼ�
	LastAttacker = Cast<ADSCharacter>(DamageCauser);

	// �������� ������ �˱����ؼ�
	FVector VictimDir = GetActorLocation() - (GetActorLocation() + GetActorForwardVector());
	FVector AttackerDir = DamageCauser->GetActorLocation() - (DamageCauser->GetActorLocation() + DamageCauser->GetActorForwardVector());
	
	float DotFB = FVector::DotProduct(VictimDir.GetSafeNormal(), AttackerDir.GetSafeNormal());
	if (DotFB >= -1 && DotFB < -0.7)
	{
		DSLOG(Warning, TEXT("Front"));
		//����Ʈ ���� �ִϸ��̼� �����ϰ� ����
		DSAnim->PlayHitReactionFront();
		return FinalDamage;
	}
	else if (DotFB >= 0.7 && DotFB < 1)
	{
		DSLOG(Warning, TEXT("Back"));
		//�� ���� �ִϸ��̼� �����ϰ� ����
		DSAnim->PlayHitReactionBack();
		return FinalDamage;
	}

	FVector CorssFB = FVector::CrossProduct(VictimDir.GetSafeNormal(), AttackerDir.GetSafeNormal());
	if (CorssFB.Z < 0)
	{
		DSLOG(Warning, TEXT("Right"));
		//Right ���� �ִϸ��̼� �����ϰ� ����
		DSAnim->PlayHitReactionLeft();
		return FinalDamage;
	}
	else if (CorssFB.Z > 0)
	{
		DSLOG(Warning, TEXT("Left"));
		//Left ���� �ִϸ��̼� �����ϰ� ����
		DSAnim->PlayHitReactionRight();
		return FinalDamage;
	}

	DSLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	return FinalDamage;
	
}


// �޺� ������ �˸��� �Լ� �޺����ݿ� �ʿ��� �⺻ ������ �����Ѵ�.
void ADSCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	// �ѹ� �������� �ٽ� �ʱ�ȭ  �������
	IsComboInputOn = false;

	// ���� �޺��� 0~3 ���̿� �ִ��� ������ ���� �ȵǴϱ�
	DSCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1))

	// ���Եǵ����δ� ����..
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// ������ ������ �޺����� ���õ� ���� �ʱ�ȭ���ִ� �Լ�
void ADSCharacter::AttackEndComboState()
{

	// ���� ��Ÿ�� ���� ������ �� �� �ʱ�ȭ
	IsComboInputOn = false; // �޺��Է¿���
	CanNextCombo = false; // ���� �޺��� �̵� ���� ����
	CurrentCombo = 0; // ���� �޺� 0������
}

void ADSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::eNomal);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		eControlMode = EControlMode::eNPC;
		GetCharacterMovement()->MaxWalkSpeed = 360.0f;
	}
}

void ADSCharacter::StartRun()
{

	if (IsStun()) return;

	DSAnim->SetRunInputCheck(true);
	
	m_bPressedRun = true;

	if (IsGuard())
	{
		//m_bGuard = false;
		SetGuard(false);
	}

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
}

void ADSCharacter::StopRun()
{
	DSAnim->SetRunInputCheck(false);

	m_bPressedRun = false;

	if (m_bPressedGuard)
	{
		StartGuard();
	}

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 360.0f;
	}
}


void ADSCharacter::StartGuard()
{
	if (IsStun()) return;

	//m_bPressedMouseRight = true;

	/*if (m_bPressedMouseLeft && m_bPressedMouseRight)
	{
		DSCHECK(IsParrying);
		Parrying();
	}*/
	//else
	//{
		//DSLOG(Warning, TEXT("StartGuard"));

		DSAnim->SetGuardInputCheck(true);

		m_bPressedGuard = true;

		SetGuard(true);


		if (IsPlayerControlled())
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 180.0f;
		}
	//}

	//m_bPressedMouseRight = false;
}

void ADSCharacter::StopGuard()
{
	//DSLOG(Warning, TEXT("StopGuard"));

	m_bPressedGuard = false;

	SetGuard(false);


	DSAnim->SetGuardInputCheck(false);


	if (m_bPressedRun) return;

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 360.0f;
	}
}


bool ADSCharacter::GetRunInputCheck()
{
	return DSAnim->GetRunInputCheck();
}


void ADSCharacter::ForwardRoll()
{
	if (IsStun()) return;

	if (IsGuard())
	{
		SetGuard(false);
	}

	m_bRolling = true;

	//StopGuard();

	DSAnim->PlayRollMontage();


}

EControlMode ADSCharacter::GetCurrentControlMode()
{
	return eControlMode;
}

// ���� IsAttacking�̶��� ȣȯ�� �ȵǳ�?
bool ADSCharacter::IsCharacterAttacking()
{
	bool bResult = false;

	if (IsAttacking)
	{
		bResult = true;
	}

	return bResult;
}

bool ADSCharacter::FunctionIsDead()
{


	return DSAnim->FunctionIsDead();
}

UDSAnimInstance* ADSCharacter::GetDSAnim()
{
	return DSAnim;
}



void ADSCharacter::SetAttackComboType(int nValue)
{
	nAttackComboType = nValue;
	DSAnim->SetAttackComboType(nAttackComboType);
	
	if (IsPlayerControlled())
	{
		DSLOG(Warning, TEXT("SetAttackComboType : %d"), nValue);
	}
}

int32 ADSCharacter::GetAttackComboType()
{
	return nAttackComboType;
}

bool ADSCharacter::IsGuard()
{
	return m_bGuard;
}

void ADSCharacter::SetGuard(bool bValue)
{
	m_bGuard = bValue;
	
	if (bValue)
	{
		// Ʈ��� �ݸ��� Ű��
		CurShield->SetActorEnableCollision(true);
		DSLOG(Warning, TEXT("Shield Collision = true!!"));
	}
	else
	{
		// ������ �ݸ��� ����
		CurShield->SetActorEnableCollision(false);
		DSLOG(Warning, TEXT("Shield Collision = false!!"));
	}
}

bool ADSCharacter::GetGuard()
{
	return m_bGuard;
}

void ADSCharacter::PlayKnockBack(float fDistance)
{
	FVector BackVector = GetActorForwardVector() * -1.f;
	FVector KnockBackLocation = GetActorLocation() + (BackVector * fDistance);

	SetActorLocation(KnockBackLocation);
}

void ADSCharacter::OnParrying()
{
	if (IsParrying)
	{
		return;
	}

	IsParrying = true;

	// �и� ��Ÿ�� ����
	DSAnim->PlayParryingMontage();
}

void ADSCharacter::ParryingCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 300.0f, // ĳ���� ���� 2���ͱ��� �浹 �߻����� Ȯ��
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(HitResult.GetActor());

			if (DSCharacter != nullptr)
			{
				if (DSCharacter->m_bCanParrying)
				{
					DSCharacter->DSAnim->PlayParryingHitMontage();
					DSLOG(Warning, TEXT("PlayParryingHitMontage!!!!!!!!!!!!!!!!!!!!!!!!!!"));
					DSCharacter->SetStun(true);
				}
				DSLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			}
		}
	}
}



void ADSCharacter::OnParryingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DSCHECK(IsParrying);

	IsParrying = false;
}


void ADSCharacter::MouseLeftClick()
{
	m_bPressedMouseLeft = true;

	//if (m_bPressedMouseLeft && m_bPressedMouseRight)
	//{
	//	Parrying();
	//}

	m_bPressedMouseLeft = false;
}

void ADSCharacter::MouseRightClick()
{
	m_bPressedMouseRight = true;

	//if (m_bPressedMouseLeft && m_bPressedMouseRight)
	//{
	//	Parrying();
	//}

	//m_bPressedMouseRight = false;
}

void ADSCharacter::Parrying()
{
	if (m_bGuard)
	{
		OnParrying();
	}
}

void ADSCharacter::SetStun(bool bValue)
{
	m_bStun = bValue;
}

bool ADSCharacter::IsStun()
{
	return m_bStun;
}