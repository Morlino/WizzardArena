// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "WizzardCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		// Initialize Blackboard
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			// Get the player character reference
			AWizzardCharacter* PlayerCharacter = Cast<AWizzardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (PlayerCharacter)
			{
				// Set TargetActor in Blackboard
				BlackboardComp->SetValueAsObject("TargetActor", PlayerCharacter);

				// Bind to death event
				PlayerCharacter->OnDeath.AddDynamic(this, &AEnemyAIController::OnPlayerDeath);
			}

			// Start the Behavior Tree
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void AEnemyAIController::OnPlayerDeath(ABaseCharacter* DeadCharacter)
{
	// Cast again to ensure it’s the player (optional, for safety)
	if (Cast<AWizzardCharacter>(DeadCharacter) != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Player Died!"));
		BlackboardComp->SetValueAsObject("TargetActor", nullptr);
	}
}