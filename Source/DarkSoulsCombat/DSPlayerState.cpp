// Fill out your copyright notice in the Description page of Project Settings.

#include "DSPlayerState.h"


ADSPlayerState::ADSPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
}

int32 ADSPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ADSPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ADSPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("WooSub"));
	CharacterLevel = 5;
	GameScore = 0;
}

