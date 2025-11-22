// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "WizzardCharacter.generated.h"

UCLASS()
class WIZZARDARENA_API AWizzardCharacter : public ACharacter
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

	class UWizzardHUD* WizzardHUD;

	FVector CursorWorldLocation;

	// Combat
	bool bIsFiring = false;

	float FireCurrentCooldown = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitHUD();

	void Move(const FInputActionValue& Value);

	void RotateToCursor();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Die();

	void TakeDamage(float DamageAmount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components);
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components);
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	class USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	float FireCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat);
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh);
	FRotator MeshRotationOffset = FRotator(0.0f, -90.0f, 0.0f);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHUDReference(UWizzardHUD* HUD);

	UFUNCTION()
	void ShootProjectile();

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();
};
