// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ProjectileSpawnPoint.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	void CollectSetSpawnPoints();

private:
	FTimerHandle AttackTimerHandle;

	bool bIsAttacking = false;

	bool bIsMovingToTarget = false;

	FTimerHandle ProjectileCooldownTimer;

	bool bCanProjectileAttack = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthWidget")
	class UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AggroRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 1.0f;

	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* ProjectileMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TArray<UProjectileSpawnPoint*> ProjectileSpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectilePushStrength = 500.f;

	// FUNCTIONS
	// Core
	void BeginPlay();

	UFUNCTION()
	void UpdateHealthWidget(float NewHealth, float NewMaxHealth);

	// Movement
	UFUNCTION(BlueprintCallable, Category="Combat")
	void MoveToTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StopMoving();

	// Attack
	UFUNCTION()
	void ResetAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void TraceAttack(float TraceRadius, FVector TraceOffset);
	
	// Projectiles
	UFUNCTION(BlueprintCallable, Category="Combat")
	void ShootProjectiles();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ProjectileAttack();
	void ResetProjectileCooldown();
};
