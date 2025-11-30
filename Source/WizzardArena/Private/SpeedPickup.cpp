// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPickup.h"

#include "WizzardCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASpeedPickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);
	if (!Player) return;

	Player->ApplySpeedBoost(SpeedBoostAmount);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[Player, this]()
		{
			Player->RemoveSpeedBoost(SpeedBoostAmount);
		},
		BoostDuration,
		false
	);

	Destroy();
}
