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
		CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		DSLOG(Error, TEXT("Level (%d) data dosen't exist"), NewLevel);
	}
}

void UDSCharacterStatComponent::SetDamage(float NewDamage)
{
	DSCHECK(nullptr != CurrentStatData);
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float UDSCharacterStatComponent::GetAttack()
{
	DSCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

