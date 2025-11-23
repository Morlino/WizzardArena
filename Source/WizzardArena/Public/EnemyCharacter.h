// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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

private:
	FTimerHandle AttackTimerHandle;

	bool bIsAttacking = false;

	bool bIsMovingToTarget = false;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AggroRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 1.0f;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack(AActor* TargetActor);

	UFUNCTION()
	void ResetAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void MoveToTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StopMoving();
};
