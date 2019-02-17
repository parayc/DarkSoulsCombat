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
	LastAttack = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LASTATTACK"));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Primary/FX/P_Greystone_Primary_Impact.P_Greystone_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
	{
		HitEffect->SetTemplate(P_HITEFFECT.Object);
		HitEffect->bAutoActivate = false;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_LASTATTACKEFFECT(TEXT("/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/LeapAOE/FX/P_Greystone_LeapAOE_GroundSmash.P_Greystone_LeapAOE_GroundSmash"));
	if (P_HITEFFECT.Succeeded())
	{
		LastAttack->SetTemplate(P_LASTATTACKEFFECT.Object);
		LastAttack->bAutoActivate = false;
	}

	RootComponent = Weapon;
	HitEffect->SetupAttachment(RootComponent);
	LastAttack->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADSWeapon::BeginPlay()
{
	Super::BeginPlay();

	HitEffect->OnSystemFinished.AddDynamic(this, &ADSWeapon::OnHitEffectFinished);
	LastAttack->OnSystemFinished.AddDynamic(this, &ADSWeapon::OnLastAttackEffectFinished);

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

void ADSWeapon::OnHitEffectFinished(UParticleSystemComponent* PSystem)
{
	HitEffect->Activate(false);
}


void ADSWeapon::PlayLastAttackEffect()
{
	LastAttack->Activate(true);
}

void ADSWeapon::OnLastAttackEffectFinished(UParticleSystemComponent* PSystem)
{
	LastAttack->Activate(false);
}
