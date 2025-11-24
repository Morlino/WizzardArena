// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.h"
#include "WaveManager.generated.h"

UCLASS()
class WIZZARDARENA_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

private:
	FTimerHandle WaveTimerHandle;

	void SpawnWaveEnemies();

	int32 EnemiesRemaining = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawner*> Spawners;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentWave = 0;

	UPROPERTY(EditAnywhere)
	TArray<int32> EnemiesPerWave;

	UPROPERTY(EditAnywhere)
	int32 BossWave = 10;

	UPROPERTY(EditAnywhere)
	float TimeBetweenWaves = 5.0f;

	UFUNCTION()
	void StartNextWave();

	UFUNCTION()
	void OnEnemyKilled(ABaseCharacter* DeadEnemy);
};
