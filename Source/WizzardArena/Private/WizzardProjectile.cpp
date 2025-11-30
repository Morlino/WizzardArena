// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"

// Sets default values
AWizzardProjectile::AWizzardProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile setup and collision
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	RootComponent = ProjectileMesh;

	// Movement on Spawn
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	ProjectileVFX->SetupAttachment(ProjectileMesh);
	ProjectileVFX->bAutoActivate = true;
}

// Called when the game starts or when spawned
void AWizzardProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpawn);
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AWizzardProjectile::OnHit);
}

// Called every frame
void AWizzardProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWizzardProjectile::ApplyPushBack(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	FVector PushDirection = OtherActor->GetActorLocation() - GetActorLocation();
	PushDirection.Z = 0;
	PushDirection.Normalize();

	// Option 1: If hit component simulates physics
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(PushDirection * PushStrength, NAME_None, true);
	}
	else if (ACharacter* HitCharacter = Cast<ACharacter>(OtherActor))
	{
		// Option 2: For characters without physics
		FVector LaunchVelocity = PushDirection * PushStrength;
		HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
	}
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

	ApplyPushBack(OtherActor, OtherComp);

	Destroy();
}

void AWizzardProjectile::SetDamage(float InDamage)
{
	Damage = InDamage;
}

void AWizzardProjectile::SetPushStrength(float InPushStrength)
{
	PushStrength = InPushStrength;
}

void AWizzardProjectile::SetProjectileSpeed(float InProjectileSpeed)
{
	ProjectileSpeed = InProjectileSpeed;

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = ProjectileSpeed;
		ProjectileMovement->MaxSpeed = ProjectileSpeed;

		// If projectile already moved, update its velocity
		ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * ProjectileSpeed;
	}
}

void AWizzardProjectile::SetProjectileOwner(AActor* InOwner)
{
	ProjectileOwner = InOwner;
}

void AWizzardProjectile::SetProjectileRadius(float InRadius)
{
	// Scale mesh and VFX uniformly
	if (ProjectileMesh)
	{
		ProjectileMesh->SetWorldScale3D(FVector(InRadius));
	}

	if (ProjectileVFX)
	{
		ProjectileVFX->SetWorldScale3D(FVector(InRadius));
	}
}

