// Fill out your copyright notice in the Description page of Project Settings.

#include "DSGameInstance.h"

UDSGameInstance::UDSGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/CharacterStat/DSCharacterData.DSCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DSCHARACTER(*CharacterDataPath);
	DSCHECK(DT_DSCHARACTER.Succeeded());
	DSCharacterTable = DT_DSCHARACTER.Object;
	DSCHECK(DSCharacterTable->GetRowMap().Num() > 0);
}

void UDSGameInstance::Init()
{
	Super::Init();
	DSLOG(Warning, TEXT("DropExp of Level 20 DSCharacter : %d"), GetDSCharacterData(20)->DropExp);
}

FDSCharacterData * UDSGameInstance::GetDSCharacterData(int32 Level)
{
	return DSCharacterTable->FindRow<FDSCharacterData>(*FString::FromInt(Level), TEXT(""));
}
