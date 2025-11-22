// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWizzardProjectile::AWizzardProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile setup and collision
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ProjectileMesh->SetGenerateOverlapEvents(true);

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
	
}

// Called every frame
void AWizzardProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

