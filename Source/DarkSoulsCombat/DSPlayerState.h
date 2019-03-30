// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "GameFramework/PlayerState.h"
#include "DSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API ADSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ADSPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;

	void InitPlayerData();

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	//Transient : 과도현상?
	UPROPERTY(Transient)
	int32 CharacterLevel;
	

};
