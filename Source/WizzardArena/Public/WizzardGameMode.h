// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WizzardGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);

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
	UPROPERTY()
	UAudioComponent* LevelMusicComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* VictoryMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* LevelMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UWaveWidget> WaveWidgetClass;

	UPROPERTY(BlueprintAssignable, Category="Game")
	FOnGameEnd OnGameWon;

	UPROPERTY(BlueprintAssignable, Category="Game")
	FOnGameEnd OnGameLost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cutscene")
	class ULevelSequence* WinSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cutscene")
	class ULevelSequence* IntroSequence;
	
	void PlayIntroSequence();
	void PlayWinSequence();

	UFUNCTION()
	void HandleLastDefaultWave();

	UFUNCTION()
	void HandleBossDefeated();
};
