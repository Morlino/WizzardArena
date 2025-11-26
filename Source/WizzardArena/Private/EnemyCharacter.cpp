// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "WizzardHealthWidget.h"
#include "WizzardProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Components/WidgetComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyAIController::StaticClass();

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);

	HealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthWidgetComponent->SetDrawSize(FVector2D(150.f, 20.f));

}

void AEnemyCharacter::CollectSetSpawnPoints()
{
	// Clear in case of previous entries
	ProjectileSpawnPoints.Empty();

	// Collect only your custom projectile arrows
	TArray<UProjectileSpawnPoint*> FoundArrows;
	GetComponents<UProjectileSpawnPoint>(FoundArrows);

	for (UProjectileSpawnPoint* Arrow : FoundArrows)
	{
		if (Arrow)
		{
			ProjectileSpawnPoints.Add(Arrow);
			UE_LOG(LogTemp, Log, TEXT("Added ProjectileSpawnPoint: %s"), *Arrow->GetName());
		}
	}

	if (ProjectileSpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ProjectileSpawnPoints found on %s!"), *GetName());
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged.AddDynamic(this, &AEnemyCharacter::UpdateHealthWidget);

	CollectSetSpawnPoints();
}

void AEnemyCharacter::UpdateHealthWidget(float NewHealth, float NewMaxHealth)
{
    if (!HealthWidgetComponent) return;

    UWizzardHealthWidget* HealthWidget = Cast<UWizzardHealthWidget>(HealthWidgetComponent->GetUserWidgetObject());

    if (HealthWidget)
    {
        HealthWidget->SetHealth(NewHealth, NewMaxHealth);
    }
}

void AEnemyCharacter::Attack(AActor* TargetActor)
{
	if (bIsAttacking) return;

	bIsAttacking = true;

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::ResetAttack, AttackCooldown, false);
}

void AEnemyCharacter::ResetAttack()
{
	bIsAttacking = false;
}

void AEnemyCharacter::MoveToTarget(AActor* TargetActor)
{
	if (!TargetActor) return;
	
	bIsMovingToTarget = true;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToActor(TargetActor, 50.0f, true);
	}
	
}

void AEnemyCharacter::StopMoving()
{
	bIsMovingToTarget = false;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}
}

void AEnemyCharacter::TraceAttack(float TraceRadius, FVector TraceOffset)
{
	float TraceDistance = 150.f;

	// Start location is character location + offset
	FVector Start = GetActorLocation() + TraceOffset;
	FVector ForwardVector = GetActorForwardVector();
	FVector End = Start + (ForwardVector * TraceDistance);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bHit && HitResult.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			Damage,
			GetController(),
			this,
			nullptr
		);
	}
}

void AEnemyCharacter::ShootProjectiles()
{
	UE_LOG(LogTemp, Log, TEXT("ShootProjectiles called!"));

	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is null!"));
		return;
	}

	if (ProjectileSpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileSpawnPoints array is empty!"));
		return;
	}

	for (int32 i = 0; i < ProjectileSpawnPoints.Num(); i++)
	{
		UProjectileSpawnPoint* SpawnPoint = ProjectileSpawnPoints[i];
		if (!SpawnPoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("ProjectileSpawnPoints[%d] is null!"), i);
			continue;
		}

		FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = SpawnPoint->GetComponentRotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
		if (!Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile at arrow %d!"), i);
			continue;
		}

		if (auto EnemyProjectile = Cast<AWizzardProjectile>(Projectile))
		{
			EnemyProjectile->SetDamage(Damage);
			EnemyProjectile->SetPushStrength(PushStrength);
			EnemyProjectile->SetProjectileOwner(this);
		}
	}
}
