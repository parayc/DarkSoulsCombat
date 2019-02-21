// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "DSWeapon.h"
#include "Sound/SoundCue.h"
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

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ADSWeapon* CurWeapon;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* TargetUI;

	UPROPERTY(BlueprintReadOnly)
	ADSCharacter* CameraTarget;

	virtual void PostInitializeComponents() override;

	// 공격 관련
	void Attack();
	void AttackCheck();

	void AttackStartComboState();
	void AttackEndComboState();

	UAudioComponent* FootStepAudioComponent;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


private:
	EControlMode eControlMode = EControlMode::eNomal;

	// PlayerInput
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ModeChange();
	void Target();

	void SetControlMode(EControlMode eControlMode);
	
	// 범위내에서 가장 가까이 있는 놈 얻어오기
	void GetTarget();

	void RadialDetection(float DeltaTime);
	
	// 걷기 사운드 큐
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Sounds, Meta = (AllowPrivateAccess = true))
	USoundCue* FootStepSoundCue;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UDSCharacterStatComponent* CharacterStat;

	UPROPERTY()
	class UDSAnimInstance* DSAnim;





	// 공격 관련
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		USoundCue* AttackSoundCue;

	UAudioComponent* AttackAudioComponent;



};
