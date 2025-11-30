// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "WizzardCharacter.generated.h"

UCLASS()
class WIZZARDARENA_API AWizzardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizzardCharacter();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"));
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"));
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"));
	class UInputAction* DashAction;

	// Initial Setup
	class UWizzardHUD* WizzardHUD;
	FVector CursorWorldLocation;

	// Projectile
	bool bIsFiring = false;
	bool bCanFire = true;
	float FireCurrentCooldown = 0.0f;
	FTimerHandle FireCooldownTimer;

	// Dash
	bool bIsDashing = false;
	FVector DashStart;
	FVector DashTarget;
	float DashElapsedTime = 0.f;
	FTimerHandle DashCooldownTimer;
	bool bCanDash = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateHealthHUD(float NewHealth, float NewMaxHealth);

	void Move(const FInputActionValue& Value);
	void UpdateCursorWorldLocation();

	void RototatePlayerToCursor();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// PROPERTIES
	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TargetArmLength = 1800.0f;

	// Mesh Related
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	FRotator MeshRotationOffset = FRotator(0.0f, -90.0f, 0.0f);

	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	class USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	class UAnimMontage* CastAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float ProjectileCastTime = 0.6f;

	// Dash Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	class USphereComponent* DashCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashDamageRadius = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	class UAnimMontage* DashAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Condition")
	bool HasWon = false;

	// Barrier
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Powerups")
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* BreakShieldSound;

	UPROPERTY(BlueprintReadWrite, Category="Powerups")
	bool bHasShield = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Powerups")
	int32 ShieldHitsRemaining = 0;

	// Rapid Fire
	int32 ActiveRapidFireBuffs = 0;
	float BaseProjectileCastTime = 0.f;

	// Speed Boost
	int32 ActiveSpeedBuffs = 0;
	float BaseWalkSpeed = 0.f;

	// FUNCTIONS
	// Utility
	virtual void Tick(float DeltaTime) override;

	void Heal(float HealingAmount);

	// Setters
	UFUNCTION()
	void SetHUDReference(UWizzardHUD* HUD);

	// Projectile
	UFUNCTION()
	void ShootProjectile();

	void PlayCastMontageIfPossible();

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	UFUNCTION()
	void TryFire();

	UFUNCTION()
	void ResetFireCooldown();

	// Dash
	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void ResetDashCooldown();

	UFUNCTION()
	void HandleDash(float DeltaTime);

	UFUNCTION()
	void HandleWin();

	// Powerups
	void ActivateShield(int32 Hits);

	void BreakShield();

	void ApplyRapidFire(float Multiplier);

	void RemoveRapidFire();
	void ApplySpeedBoost(float Amount);
	void RemoveSpeedBoost(float Amount);
};
