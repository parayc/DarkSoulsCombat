// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacterStatComponent.h"
#include "DSGameInstance.h"

// Sets default values for this component's properties
UDSCharacterStatComponent::UDSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	Level = 1;
	// ...
}


// Called when the game starts
void UDSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDSCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}


// Called every frame
void UDSCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDSCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto DSGameInstance = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	DSCHECK(nullptr != DSGameInstance);
	CurrentStatData = DSGameInstance->GetDSCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		DSLOG(Warning, TEXT("Level dsadasdasdsat"));
	}
	else
	{
		DSLOG(Error, TEXT("Level (%d) data dosen't exist"), NewLevel);
	}
}

void UDSCharacterStatComponent::SetDamage(float NewDamage)
{
	DSCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	//if (CurrentHP <= 0.0f)
	//{
	//	OnHPIsZero.Broadcast();
	//}
}

float UDSCharacterStatComponent::GetAttack()
{
	DSCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

void UDSCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UDSCharacterStatComponent::GetHPRatio()
{
	DSCHECK(nullptr != CurrentStatData, 0.0f);

	// KINDA_SMALL_NUMBER 0이 아니지만 0과 정말 얼마 차이안나는 0.000~1과 같은 수를 그냥 0으로 보겠다 같은 내용에 쓴다
	// 0.0, 0.1, 0.2 ~ 1.0 까지 범위 나타내려고
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}