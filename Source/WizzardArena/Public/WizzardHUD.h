// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WizzardHUD.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UWizzardHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UWizzardHealthWidget* HealthWidget;

	UPROPERTY(meta = (BindWidget))
	class UWaveWidget* WaveWidget;

	UPROPERTY(meta = (BindWidget))
	class UPowerUpsWidget* PowerUpsWidget;

	void InitializeWidgets();
	
	void SetHealth(float Current, float Max);
	
	void SetShieldHits(int32 Hits);

	void SetRapidFireBuffs(int32 Count);

	void SetSpeedBuffs(int32 Count);
};
