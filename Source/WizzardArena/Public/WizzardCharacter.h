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
	float FireCurrentCooldown = 0.0f;

	// Dash
	bool bIsDashing = false;
	FVector DashStart;
	FVector DashTarget;
	float DashElapsedTime = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateHealthHUD(float NewHealth, float NewMaxHealth);

	void Move(const FInputActionValue& Value);

	void RotateToCursor();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// PROPERTIES
	// Camera Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components);
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components);
	class USpringArmComponent* SpringArmComp;

	// Mesh Related
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh);
	FRotator MeshRotationOffset = FRotator(0.0f, -90.0f, 0.0f);

	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	class USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	float FireCooldown = 1.0f;

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

	// FUNCTIONS
	// Utility
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Utilities")
	FVector GetMeshForwardVector() const;

	// Setters
	UFUNCTION()
	void SetHUDReference(UWizzardHUD* HUD);

	// Projectile
	UFUNCTION()
	void ShootProjectile();

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	// Dash
	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void HandleDash(float DeltaTime);
};
