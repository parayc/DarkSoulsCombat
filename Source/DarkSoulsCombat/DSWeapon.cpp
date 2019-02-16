// Fill out your copyright notice in the Description page of Project Settings.

#include "DSWeapon.h"

// Sets default values
ADSWeapon::ADSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	// 콜리전 셋팅
	Weapon->SetCollisionProfileName(TEXT("DSWeapon"));

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_WildImpact_01.Par_WildImpact_01"));
	if (P_HITEFFECT.Succeeded())
	{
		HitEffect->SetTemplate(P_HITEFFECT.Object);
		HitEffect->bAutoActivate = false;
		
	}

	RootComponent = Weapon;
	HitEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADSWeapon::BeginPlay()
{
	Super::BeginPlay();

	HitEffect->OnSystemFinished.AddDynamic(this, &ADSWeapon::OnEffectFinished);
}

// Called every frame
void ADSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSWeapon::PlayHitEffect()
{
	HitEffect->Activate(true);
}

void ADSWeapon::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	PSystem->Activate(false);
}

