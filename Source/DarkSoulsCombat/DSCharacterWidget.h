// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DarkSoulsCombat.h"
#include "Blueprint/UserWidget.h"
#include "DSCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULSCOMBAT_API UDSCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UDSCharacterStatComponent* NewCharacterStat);

private:
	TWeakObjectPtr<class UDSCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();


};
