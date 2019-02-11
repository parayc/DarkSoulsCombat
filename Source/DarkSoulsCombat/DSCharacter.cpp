// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADSCharacter::ADSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Git Test Code
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DSCharacter"));


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

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

	SetControlMode(eControlMode);
}

// Called when the game starts or when spawned
void ADSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ADSCharacter::UpDown(float NewAxisValue)
{

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	// GetActorForwardVector() ������ �������� �� ��������
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
}


void ADSCharacter::LeftRight(float NewAxisValue)
{
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


	if (bResult)
	{
		if (arrOverlapResults.Num() >= 0)
		{
			for (auto OverlapResult : arrOverlapResults)
			{
				ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
				if (DSCharacter)
				{
					float Dot = FVector::DotProduct(GetActorForwardVector(), (DSCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal());

					if (Dot > ClosestDotToCenter)
					{
						ClosestDotToCenter = Dot;
						CameraTarget = DSCharacter;
					}
				}
			}
		}
	}
	else
	{
		CameraTarget = nullptr;
	}
}