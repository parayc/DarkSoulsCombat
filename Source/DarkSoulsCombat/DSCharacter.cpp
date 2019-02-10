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

	// 소프트웨어 3차원 좌표계와 언리얼 엔진 3차원 좌표계가 달라서 맞춰주려고 -90,
	// 언리얼엔진에서 엑터의 기준위치는 엑터의 정중앙이지만 캐릭터 에셋은 발바닥부터 그렇기 절반 높이 -
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	// 스프링암즈 길이
	SpringArm->TargetArmLength = 450.0f;
	
	// 기존 회전각에 대한 회전 각 +, -;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	
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
	
}

// Called every frame
void ADSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float fDetectRadius = 600.0f;

	// 반경 6미터에 모든 오브젝트 탐지
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


	if (bResult)
	{
		if (arrOverlapResults.Num() >= 0)
		{
			for (auto OverlapResult : arrOverlapResults)
			{
				ADSCharacter* DSCharacter = Cast<ADSCharacter>(OverlapResult.GetActor());

				if (DSCharacter)
				{
					DrawDebugSphere(GetWorld(), GetActorLocation(), fDetectRadius, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(GetWorld(), DSCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(GetWorld(), this->GetActorLocation(), DSCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);

					return;
				}
			}
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), fDetectRadius, 16, FColor::Red, false, 0.2f);
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
	
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
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
	switch (eMode)
	{
	case EControlMode::eNomal:
	{
		SpringArm->TargetArmLength = 450.f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

		break;
	}

	case EControlMode::eDarkSouls:
	{
		SpringArm->TargetArmLength = 450.f;
		SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

		break;
	}

	default:
		break;
	}
}
