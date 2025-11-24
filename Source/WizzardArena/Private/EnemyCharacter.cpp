// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "WizzardHealthWidget.h"
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

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged.AddDynamic(this, &AEnemyCharacter::UpdateHealthWidget);
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

	if (TargetActor)
	{
		FVector ThisActorLocation = GetActorLocation();
		FVector TargetLocation = TargetActor->GetActorLocation();
		float Distance = FVector::Dist(ThisActorLocation, TargetLocation);

		if (Distance <= AttackRange)
		{
			UGameplayStatics::ApplyDamage(TargetActor, Damage, GetController(), this, nullptr);
		}
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