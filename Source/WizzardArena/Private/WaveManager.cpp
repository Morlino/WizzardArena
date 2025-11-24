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
	if (!EnemiesPerWave.IsValidIndex(CurrentWave - 1)) return;

	int32 Count = EnemiesPerWave[CurrentWave - 1];
	EnemiesRemaining = Count;

	int32 NumSpawners = Spawners.Num();
	if (NumSpawners == 0) return;

	int32 BasePerSpawner = Count / NumSpawners;
	int32 Remainder = Count % NumSpawners;

	for (int32 i = 0; i < NumSpawners; i++)
	{
		if (Spawners[i])
		{
			int32 ToSpawn = BasePerSpawner;
			if (i < Remainder) ToSpawn++;
			if (ToSpawn > 0)
				Spawners[i]->SpawnWave(ToSpawn);
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

