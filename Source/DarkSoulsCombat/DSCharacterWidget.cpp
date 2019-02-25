// Fill out your copyright notice in the Description page of Project Settings.

#include "DSCharacterWidget.h"
#include "DSCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UDSCharacterWidget::BindCharacterStat(UDSCharacterStatComponent * NewCharacterStat)
{
	DSCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UDSCharacterWidget::UpdateHPWidget);

}


void UDSCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	DSCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UDSCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}