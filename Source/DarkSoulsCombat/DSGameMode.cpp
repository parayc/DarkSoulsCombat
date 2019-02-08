// Fill out your copyright notice in the Description page of Project Settings.

#include "DSGameMode.h"
#include "DSCharacter.h"
#include "DSPlayerController.h"

ADSGameMode::ADSGameMode()
{
	DefaultPawnClass = ADSCharacter::StaticClass();
	PlayerControllerClass = ADSPlayerController::StaticClass();
}