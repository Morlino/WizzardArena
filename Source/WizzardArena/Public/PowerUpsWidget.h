// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PowerUpsWidget.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UPowerUpsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ShieldText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* RapidFireText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SpeedText;
	
public:
	void UpdateShieldHits(int32 ShieldHitsRemaining);
	void UpdateRapidFireBuffs(int32 ActiveRapidFireBuffs);
	void UpdateSpeedBuffs(int32 ActiveSpeedBuffs);
};
