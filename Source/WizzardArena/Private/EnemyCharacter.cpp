// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyAIController::StaticClass();
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