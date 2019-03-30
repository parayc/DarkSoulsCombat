// Fill out your copyright notice in the Description page of Project Settings.

#include "DSGameMode.h"
#include "DSCharacter.h"
#include "DSPlayerController.h"
#include "DSPlayerState.h"

ADSGameMode::ADSGameMode()
{
	DefaultPawnClass = ADSCharacter::StaticClass();
	PlayerControllerClass = ADSPlayerController::StaticClass();
	PlayerStateClass = ADSPlayerController::StaticClass();
}

void ADSGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto DSPlayerState = Cast<ADSPlayerState>(NewPlayer->PlayerState);
	DSCHECK(nullptr != DSPlayerState);
	DSPlayerState->InitPlayerData();

}