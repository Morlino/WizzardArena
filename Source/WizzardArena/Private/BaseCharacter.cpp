// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Stats
	CurrentHealth = MaxHealth;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Die_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("%s Died"), *GetName());
    
	if (OnDeath.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("OnDeath is bound! Broadcasting..."));
		OnDeath.Broadcast(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OnDeath has NO bindings"));
	}
    
	Destroy();
}


float ABaseCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Take Damage %f"), DamageAmount);
	float ActualDamage = FMath::Clamp(DamageAmount, 0.f, MaxHealth);
	CurrentHealth -= ActualDamage;

	PlayAnimMontage(GetHitMontage);

	// Broadcast an event so anyone listening (HUD, effects, etc.) can respond
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return ActualDamage;
}

float ABaseCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

float ABaseCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void ABaseCharacter::SetCurrentHealth(float inCurrentHealth)
{
	CurrentHealth = inCurrentHealth;
}

void ABaseCharacter::SetMaxHealth(float inMaxHealth)
{
	MaxHealth = inMaxHealth;
}
