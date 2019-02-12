// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "DSAIController.h"
#include "DSWeapon.h"
#include "DSAnimInstance.h"

// Sets default values
ADSCharacter::ADSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADSAIController::StaticClass();
	// 자동으로 플레어아가 소유한 캐릭터를 제외하고는 AICotroller가 소유하도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DSCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	TargetUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("TARGETUI"));


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	TargetUI->SetupAttachment(GetMesh());
	
	// TargetLockOn UI를 모든 캐릭터마다 만들고 그 숨김을 기본으로 해놓는다.
	TargetUI->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	TargetUI->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_TARGETLOCKON(TEXT("/Game/Enemy/UI/UI_TargetLockOn.UI_TargetLockOn_C"));
	if (UI_TARGETLOCKON.Succeeded())
	{
		TargetUI->SetWidgetClass(UI_TARGETLOCKON.Class);
		TargetUI->SetDrawSize(FVector2D(35.0f, 35.0f));
	}

	TargetUI->SetHiddenInGame(true);

	// 소프트웨어 3차원 좌표계와 언리얼 엔진 3차원 좌표계가 달라서 맞춰주려고 -90,
	// 언리얼엔진에서 엑터의 기준위치는 엑터의 정중앙이지만 캐릭터 에셋은 발바닥부터 그렇기 절반 높이 -
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	GetCharacterMovement()->JumpZVelocity = 600.f;



	// 스켈레탈 메시 연결
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	// 메쉬의 애니메이션은 애님 블루프린트로 관리할 것
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 애니메이션 블루프린트 연결 
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
	

	FName Socket_RightHand_Weapon(TEXT("Socket_RightHand_Weapon"));
	auto CurWeapon = GetWorld()->SpawnActor<ADSWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket_RightHand_Weapon);
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

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	// GetActorForwardVector() 액터의 전진방향 값 가져오기
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
}


void ADSCharacter::LeftRight(float NewAxisValue)
{
	// GetActorRightVector() 액터의 옆방향 값 가져오기
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
	// 길이 설정
	SpringArm->TargetArmLength = 450.f;

	// 높이, 회전 설정
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

	// 폰이 카메라 로테이션을 제어할건지
	SpringArm->bUsePawnControlRotation = true;

	// 부모(캡슐의) X,Y,Z정보 상속받아서 그대로 쓸건지
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// 카메라의 시야 중간에 방해물이 있으면 그 앞까지 시야 땡길건지
	SpringArm->bDoCollisionTest = true;	
	
	// 스프링 암즈에 달린 카메라가 스프링 암즈보다 한박자 느리게 따라오도록 하기 위한 옵션
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->CameraLagSpeed = 1.f;
	SpringArm->CameraRotationLagSpeed = 1.f;
	SpringArm->CameraLagMaxDistance = 100.f;

	switch (eMode)
	{

	case EControlMode::eNomal:
	{

		// 컨트롤 회전의 Yaw축과 Pawn의 Yaw과 연동시켜줄지 확인하는 기능 true면 연동
		// 예를들어 이걸 false로 지정하면 카메라가 좌우로 움직여도 카메라 시점만 이동하고
		// 캐릭터는 그 방향으로 돌아가지 않음.
		bUseControllerRotationYaw = false;

		// 캐릭터가 움직으는 방향으로 캐릭터를 자동으로 돌려줄건지
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

	//// 이것도 계속 돌고 있는게 맞다.
	//// 반경 6미터에 모든 오브젝트 탐지
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

	//// 탐지된 오브젝트가 캐릭터인지 판별하고
	//// 범위 표시 그린, 탐지 오브젝트와의 선 그리기
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


	// 이것은 틱마다 계솔 돌아가야하는 기느잉 맞아
	// 카메라가 타겟을 바라보도록 만드는 부분
	if (CameraTarget != nullptr)
	{
		FVector TargetVect = CameraTarget->GetActorLocation() - GetActorLocation();
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 10.f);

		// 컨트롤러의(마우스 방향)시점 과 캐릭터의 시선방향이 일치하는 옵션을 켜놓았기에 컨트롤러의 방향을 정해주면 캐릭터가 자동으로 그곳을 바라본다.
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

	// Tab키를 누르면 범위내의 가장 가까운 타겟을 CameraTarget으로 설정해준다.
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

	// 탐색된 결과가 존재한다면
	if (bResult)
	{
		// 탐색된 결과가 들어있는 배열에 1개 이상의 값이 들어있다면
		if (arrOverlapResults.Num() >= 0)
		{
			// 배열의 수만큼 반복
			for (auto OverlapResult : arrOverlapResults)
			{
				ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());
				if (DSCharacter)
				{
					// 탐색된 액터의 위치와 플레이어 캐릭터의 위치를 빼서 나온 백터값과
					// 카메라의 정면 방향 벡터의 내적을 구하여
					// 수치가 가장 높다면 카메라 정면 방향에 가장 근접한 오브젝트 판별
					float Dot = FVector::DotProduct(Camera->GetForwardVector(), (DSCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal());

					if (Dot > ClosestDotToCenter)
					{
						ClosestDotToCenter = Dot;
						CameraTarget = DSCharacter;
						
					}
				}
			}

			// 이전 타겟과 현재 선택된 타겟이 다르다면 이전 타겟의 TargetLockOn UI를 숨김으로 변경
			if (PrevCameraTarget != CameraTarget && PrevCameraTarget != nullptr)
			{
				PrevCameraTarget->TargetUI->SetHiddenInGame(true);
			}

			// 타겟이 정해졌다면 타겟의 TargetLockOn UI를 oN
			if (CameraTarget != nullptr)
			{
				CameraTarget->TargetUI->SetHiddenInGame(false);
			}
		}
	}
	else
	{
		// 범위내의 선택된 타겟이 없다면
		CameraTarget = nullptr;

		// 이전 선택 타겟이 있었을 경우 이전 타겟의 TargetLockOn UI를 숨김으로 변경
		if (PrevCameraTarget != CameraTarget && PrevCameraTarget != nullptr)
		{
			PrevCameraTarget->TargetUI->SetHiddenInGame(true);
		}
	}
}

// 일반공격 함수
void ADSCharacter::Attack()
{
	auto AnimInstance = Cast<UDSAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;
	{
		AnimInstance->PlayAttackMontage();
	}
}