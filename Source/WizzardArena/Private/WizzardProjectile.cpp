// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWizzardProjectile::AWizzardProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile setup and collision
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	RootComponent = ProjectileMesh;

	// Movement on Spawn
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AWizzardProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AWizzardProjectile::OnHit);
}

// Called every frame
void AWizzardProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWizzardProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner())
		return; // ignore self

	UE_LOG(LogTemp, Log, TEXT("Projectile hit: %s"), *OtherActor->GetName());
	
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			HitEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}

	// Apply damage
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);

	Destroy();
}

