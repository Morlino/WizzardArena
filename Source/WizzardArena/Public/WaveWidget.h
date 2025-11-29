// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveWidget.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UWaveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWave(int32 WaveNumber);

	UFUNCTION(BlueprintCallable)
	void SetEnemiesRemaining(int32 EnemiesLeft);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* WaveText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* EnemiesText;
};