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

void AEnemySpawner::SpawnSpecificEnemy(TSubclassOf<AEnemyCharacter> InEnemyClass)
{
	if (!SpawnVolume || !InEnemyClass) return;
    
	const FVector Origin = SpawnVolume->GetComponentLocation();
	const FVector Extent = SpawnVolume->GetScaledBoxExtent();

	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(
		InEnemyClass, 
		SpawnLocation,
		FRotator::ZeroRotator
	);

	if (Enemy && WaveManager)
		Enemy->OnDeath.AddDynamic(WaveManager, &AWaveManager::OnEnemyKilled);
}
