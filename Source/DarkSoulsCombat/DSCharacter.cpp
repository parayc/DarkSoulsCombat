// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "DSAIController.h"
#include "DSAnimInstance.h"

// Sets default values
ADSCharacter::ADSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADSAIController::StaticClass();
	// �ڵ����� �÷���ư� ������ ĳ���͸� �����ϰ�� AICotroller�� �����ϵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// �ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DSCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	TargetUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("TARGETUI"));


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	TargetUI->SetupAttachment(GetMesh());
	
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


	static ConstructorHelpers::FObjectFinder<USoundCue> SC_AttackSoundCue(TEXT("/Game/Weapon_Pack/SwordSound/A_Sword_Swing_Cue.A_Sword_Swing_Cue"));
	if (SC_AttackSoundCue.Succeeded())
	{
		AttackSoundCue = SC_AttackSoundCue.Object;
		AttackSoundCueComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSoundCueComponent"));
		AttackSoundCueComponent->SetupAttachment(RootComponent);
	

	}

	

	// �޺������� ���� ����
	IsAttacking = false; // ���� ���� ��Ÿ�� ���������� �ľ�
	MaxCombo = 4; // �޺����� �ܰ� �ִ�ġ
	AttackEndComboState(); // ������ ������ �޺����� ���õ� ���� �ʱ�ȭ���ַ���


	SetControlMode(eControlMode);
}

void ADSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DSAnim = Cast<UDSAnimInstance>(GetMesh()->GetAnimInstance());

	DSAnim->OnAttackHit.AddUObject(this, &ADSCharacter::AttackCheck);

	DSAnim->OnMontageEnded.AddDynamic(this, &ADSCharacter::OnAttackMontageEnded);

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

			if (AttackSoundCueComponent && AttackSoundCue)
			{
				AttackSoundCueComponent->Play(0.f);
			}

		}

	});

	DSAnim->OnLastAttack.AddLambda([this]() -> void {
		CurWeapon->PlayLastAttackEffect();
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

	if (AttackSoundCueComponent && AttackSoundCue)
	{
		AttackSoundCueComponent->SetSound(Cast<USoundBase>(AttackSoundCue));

	}
}


// Called every frame
void ADSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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
}

void ADSCharacter::UpDown(float NewAxisValue)
{
	if (IsAttacking)
	{



		return;
	}

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
	AddControllerPitchInput(NewAxisValue);
}

void ADSCharacter::Turn(float NewAxisValue)
{
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
	if (CameraTarget != nullptr)
	{
		FVector TargetVect = CameraTarget->GetActorLocation() - GetActorLocation();
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 10.f);

		// ��Ʈ�ѷ���(���콺 ����)���� �� ĳ������ �ü������� ��ġ�ϴ� �ɼ��� �ѳ��ұ⿡ ��Ʈ�ѷ��� ������ �����ָ� ĳ���Ͱ� �ڵ����� �װ��� �ٶ󺻴�.
		GetController()->SetControlRotation(NewRot);

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
				CameraTarget->TargetUI->SetHiddenInGame(false);
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
		DSAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;

		if (AttackSoundCueComponent && AttackSoundCue)
		{
			AttackSoundCueComponent->Play(0.f);
		}
	}


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
		FCollisionShape::MakeSphere(50.0f),
		Params
		);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			DSLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			CurWeapon->PlayHitEffect();
		}
	}
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