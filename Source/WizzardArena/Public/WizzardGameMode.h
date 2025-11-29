// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizzardGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API AWizzardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWizzardGameMode();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* VictoryMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* LevelMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	class UWaveWidget* WaveHUDInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UWaveWidget> WaveWidgetClass;

	UFUNCTION()
	void HandleWinCondition();
};
