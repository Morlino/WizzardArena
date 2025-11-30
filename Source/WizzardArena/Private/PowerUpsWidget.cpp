// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpsWidget.h"
#include "Components/TextBlock.h"

void UPowerUpsWidget::UpdateShieldHits(int32 ShieldHitsRemaining)
{
	if (ShieldText) // Bind this to your text widget in UMG
	{
		ShieldText->SetText(FText::AsNumber(ShieldHitsRemaining));
	}
}

void UPowerUpsWidget::UpdateRapidFireBuffs(int32 ActiveRapidFireBuffs)
{
	if (RapidFireText)
	{
		RapidFireText->SetText(FText::AsNumber(ActiveRapidFireBuffs));
	}
}

void UPowerUpsWidget::UpdateSpeedBuffs(int32 ActiveSpeedBuffs)
{
	if (SpeedText)
	{
		SpeedText->SetText(FText::AsNumber(ActiveSpeedBuffs));
	}
}
