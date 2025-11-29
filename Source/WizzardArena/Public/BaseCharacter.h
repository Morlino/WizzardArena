// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewCurrentHealth, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, ABaseCharacter*, DeadCharacter);

UCLASS()
class WIZZARDARENA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDeathSignature OnDeath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* GetHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;

	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float PushStrength = 1000.0f;

	// Getters
	UFUNCTION()
	float GetCurrentHealth();

	UFUNCTION()
	float GetMaxHealth();

	// Setters
	UFUNCTION()
	void SetCurrentHealth(float inCurrentHealth);

	UFUNCTION()
	void SetMaxHealth(float inMaxHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character|Death")
	void Die();

	virtual void Die_Implementation();
};
