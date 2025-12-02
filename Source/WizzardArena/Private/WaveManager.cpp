// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"

#include "WaveWidget.h"
#include "WizzardHUD.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWaveManager::SetHUDReference(UWizzardHUD* InHUD)
{
	HUD = InHUD;
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
	// Cast to AEnemyCharacter so it matches ActiveEnemies array type
	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(DeadEnemy);
	if (EnemyChar)
	{
		ActiveEnemies.Remove(EnemyChar);
	}

	// Update remaining enemies count
	EnemiesRemaining = ActiveEnemies.Num();

	UE_LOG(LogTemp, Warning, TEXT("Enemy DIED, %d remaining"), EnemiesRemaining);

	// Update widget
	if (HUD)
	{
		HUD->SetEnemiesRemaining(EnemiesRemaining);
	}

	// Check if wave is complete
	if (EnemiesRemaining <= 0)
	{
		// If this was the final wave → trigger victory
		if (CurrentWave == Waves.Num())
		{
			OnBossWavesCompleted.Broadcast();
		}
		else if (CurrentWave == Waves.Num() - 1)
		{
			OnAllWavesCompleted.Broadcast();
		}
		else
		{
			// Otherwise wait normally before starting next wave
			GetWorldTimerManager().SetTimer(
				WaveTimerHandle,
				this,
				&AWaveManager::StartNextWave,
				TimeBetweenWaves,
				false
			);
		}
	}
}

void AWaveManager::RegisterEnemy(AEnemyCharacter* Enemy)
{
	if (!Enemy) return;
	ActiveEnemies.Add(Enemy);

	// Subscribe to death
	Enemy->OnDeath.AddDynamic(this, &AWaveManager::OnEnemyKilled);
    
	EnemiesRemaining = ActiveEnemies.Num();

	// Update widget
	if (HUD)
	{
		HUD->SetEnemiesRemaining(EnemiesRemaining);
	}
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HUD->WaveWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveWidget Exists"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WaveWidget Doesn't Exists"));
	}
}

void AWaveManager::StartNextWave()
{
	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD not yet assigned, delaying wave start"));
		// Try again next tick
		GetWorldTimerManager().SetTimerForNextTick(this, &AWaveManager::StartNextWave);
		return;
	}

	CurrentWave++;

	if (CurrentWave > Waves.Num())
	{
		OnBossWavesCompleted.Broadcast();
		return;
	}

	// Update widget for new wave
	if (HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave %d"), CurrentWave);
		HUD->SetWave(CurrentWave);
		HUD->SetEnemiesRemaining(EnemiesRemaining);
	}

	SpawnWaveEnemies();
}

