// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EnemyCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class WIZZARDARENA_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

private:
	void SpawnEnemyInVolume();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	class AWaveManager* WaveManager;

	void SpawnSpecificEnemy(TSubclassOf<AEnemyCharacter> InEnemyClass);

	void SpawnWave(int32 TotalEnemies);
};
