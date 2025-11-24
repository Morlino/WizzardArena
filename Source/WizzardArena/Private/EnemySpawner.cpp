// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;
	SpawnVolume->SetBoxExtent(FVector(200.f, 200.f, 100.f));
}

void AEnemySpawner::SpawnEnemyInVolume()
{
	if (!SpawnVolume || !EnemyClass) return;
	
	FVector Origin = SpawnVolume->GetComponentLocation();
	FVector Extent = SpawnVolume->GetScaledBoxExtent();

	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	FRotator RandomRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, RandomLocation, RandomRotation, SpawnParams);

	if (NewEnemy && WaveManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy BOUND!"));

		// Bind to wave manager
		NewEnemy->OnDeath.AddDynamic(WaveManager, &AWaveManager::OnEnemyKilled);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fild to Bind!"));

	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnWave(int32 TotalEnemies)
{
	for (int32 i = 0; i < TotalEnemies; i++)
	{
		SpawnEnemyInVolume();
	}
}