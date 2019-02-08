// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacter.h"

// Sets default values
ADSCharacter::ADSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// ����Ʈ���� 3���� ��ǥ��� �𸮾� ���� 3���� ��ǥ�谡 �޶� �����ַ��� -90,
	// �𸮾������� ������ ������ġ�� ������ ���߾������� ĳ���� ������ �߹ٴں��� �׷��� ���� ���� -
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	// ���������� ����
	SpringArm->TargetArmLength = 400.0f;
	
	// ���� ȸ������ ���� ȸ�� �� +, -;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	
	// ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
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

}

// Called to bind functionality to input
void ADSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADSCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADSCharacter::LeftRight);

}

void ADSCharacter::UpDown(float NewAxisValue)
{
	// GetActorForwardVector() ������ �������� �� ��������
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}


void ADSCharacter::LeftRight(float NewAxisValue)
{
	// GetActorRightVector() ������ ������ �� ��������
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
