// Fill out your copyright notice in the Description page of Project Settings.

#include "DSShield.h"

// Sets default values
ADSShield::ADSShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SHIELD"));

	Shield->SetCollisionProfileName(TEXT("DSShield"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SHIELD(TEXT("/Game/Weapon_Pack/Skeletal_Mesh/SK_Shield.SK_Shield"));
	if (SK_SHIELD.Succeeded())
	{
		Shield->SetSkeletalMesh(SK_SHIELD.Object);
	}

	RootComponent = Shield;

	// °È±â ¼Ò¸®
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_HitSoundCue(TEXT("/Game/SoundSoruce/SwordHit/SC_SwordMetalHit.SC_SwordMetalHit"));
	if (SC_HitSoundCue.Succeeded())
	{
		HitSoundCue = SC_HitSoundCue.Object;
		HitSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSoundAudioComponent"));
		HitSoundAudioComponent->SetupAttachment(RootComponent);
	}


}

// Called when the game starts or when spawned
void ADSShield::BeginPlay()
{
	Super::BeginPlay();

	if (HitSoundAudioComponent && HitSoundCue)
	{
		HitSoundAudioComponent->SetSound(Cast<USoundBase>(HitSoundCue));
	}
}

// Called every frame
void ADSShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSShield::PlayHitSound()
{
	HitSoundAudioComponent->Play(0.f);
}