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
	eDarkSouls = 1,
	eNPC = 2
};

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarUI;

	UPROPERTY(BlueprintReadOnly)
	ADSCharacter* CameraTarget;

	//// 캐릭터 스텟 가져오기
	//UPROPERTY(BlueprintReadOnly)
	//class UDSCharacterStatComponent* CharacterStat;

	virtual void PostInitializeComponents() override;

	// 공격 관련
	void Attack();
	void AttackCheck();

	void AttackStartComboState();
	void AttackEndComboState();

	FOnAttackEndDelegate OnAttackEnd;

	UAudioComponent* FootStepAudioComponent;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UDSCharacterStatComponent* CharacterStat;
	
	// 범위내에서 가장 가까이 있는 놈 얻어오기
	void GetTarget();

	void Target();

private:
	EControlMode eControlMode = EControlMode::eNomal;

	ADSCharacter* LastAttacker;


	// PlayerInput
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ModeChange();
	void StartRun();
	void StopRun();
	void ForwardRoll();

	void SetControlMode(EControlMode eControlMode);
	
	void RadialDetection(float DeltaTime);
	
	// 걷기 사운드 큐
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Sounds, Meta = (AllowPrivateAccess = true))
	USoundCue* FootStepSoundCue;

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
