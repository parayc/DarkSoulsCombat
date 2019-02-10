// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "GameFramework/Character.h"
#include "DSCharacter.generated.h"

enum class EControlMode
{
	eNomal = 0,
	eDarkSouls = 1
};

UCLASS()
class DARKSOULSCOMBAT_API ADSCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ADSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly)
	ADSCharacter* CameraTarget;



private:
	EControlMode eControlMode = EControlMode::eNomal;

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ModeChange();
	void SetControlMode(EControlMode eControlMode);

	void RadialDetection(float DeltaTime);
};
