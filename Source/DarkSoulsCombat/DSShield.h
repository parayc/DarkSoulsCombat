// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "GameFramework/Actor.h"
#include "DSShield.generated.h"

UCLASS()
class DARKSOULSCOMBAT_API ADSShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(VisibleAnywhere, Category = Shield)
	USkeletalMeshComponent* Shield;


};
