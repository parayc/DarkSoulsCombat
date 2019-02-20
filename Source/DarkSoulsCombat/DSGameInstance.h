// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "DSGameInstance.generated.h"

// 캐릭터의 스텟 같은건 게임 인스턴스가 관리한다
/**
 * 
 */

USTRUCT(BlueprintType)
struct FDSCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDSCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};



UCLASS()
class DARKSOULSCOMBAT_API UDSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UDSGameInstance();

	virtual void Init() override;
};
