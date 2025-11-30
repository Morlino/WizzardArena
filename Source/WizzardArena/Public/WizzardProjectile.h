// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WizzardProjectile.generated.h"

UCLASS()
class WIZZARDARENA_API AWizzardProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWizzardProjectile();

private:
	float Damage;

	float PushStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite)
	AActor* ProjectileOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	class UNiagaraComponent* ProjectileVFX;

	// Particle effect on impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	class UNiagaraSystem* HitEffect;

	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float LifeSpawn = 10.0f;
	
	// Hit handling
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void SetDamage(float InDamage);
	void SetPushStrength(float InPushStrength);
	void SetProjectileSpeed(float InProjectileSpeed);
	void SetProjectileOwner(AActor* InOwner);
	void SetProjectileRadius(float InRadius);
	void ApplyPushBack(AActor* OtherActor, UPrimitiveComponent* OtherComp);
};
