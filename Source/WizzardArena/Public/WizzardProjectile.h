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

	// Damage (optional)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float Damage = 10.f;

	// Particle effect on impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	class UNiagaraSystem* HitEffect;
	
	// Hit handling
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
