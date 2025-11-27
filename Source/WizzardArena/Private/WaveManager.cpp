// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWaveManager::SpawnWaveEnemies()
{
	if (!Waves.IsValidIndex(CurrentWave - 1))
	{
		UE_LOG(LogTemp, Error, TEXT("Wave isn't valid"));
		return;
	}

	const FWaveDefinition& Wave = Waves[CurrentWave - 1];

	// Calculate total enemies for tracking
	EnemiesRemaining = 0;
	for (const FEnemySpawnInfo& Info : Wave.Enemies)
		EnemiesRemaining += Info.Count;

	if (Spawners.Num() == 0) return;

	int SpawnerIndex = 0;

	for (const FEnemySpawnInfo& Info : Wave.Enemies)
	{
		for (int i = 0; i < Info.Count; i++)
		{
			AEnemySpawner* Spawner = Spawners[SpawnerIndex];
			if (Spawner)
				Spawner->SpawnSpecificEnemy(Info.EnemyClass);

			SpawnerIndex = (SpawnerIndex + 1) % Spawners.Num();
		}
	}
}

void AWaveManager::OnEnemyKilled(ABaseCharacter* DeadEnemy)
{
	EnemiesRemaining--;
	UE_LOG(LogTemp, Error, TEXT("Enemy DIED"));

	if (EnemiesRemaining <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("START NEXT"));
		GetWorldTimerManager().SetTimer(
			WaveTimerHandle,
			this,
			&AWaveManager::StartNextWave,
			TimeBetweenWaves,
			false
		);
	}
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

	StartNextWave();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveManager::StartNextWave()
{
	CurrentWave++;

	bool bIsBossWave = (CurrentWave == BossWave);

	if (bIsBossWave)
	{
		EnemiesRemaining = 1;
		// TODO: SpawnBoss();
	}
	else
	{
		SpawnWaveEnemies();
	}
}

