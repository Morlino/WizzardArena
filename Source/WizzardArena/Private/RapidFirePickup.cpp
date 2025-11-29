// Fill out your copyright notice in the Description page of Project Settings.


#include "RapidFirePickup.h"
#include "WizzardCharacter.h"

void ARapidFirePickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);
	if (!Player) return;

	// Save the original cast time
	const float OriginalProjectileCastTime = Player->ProjectileCastTime;

	Player->ProjectileCastTime /= CastSpeedBoostMultiplier;

	// Start timer to reset it
	FTimerHandle TimerHandle;
	Player->GetWorldTimerManager().SetTimer(
		TimerHandle,
		[Player, OriginalProjectileCastTime]()
		{
			Player->ProjectileCastTime = OriginalProjectileCastTime;
		},
		CastSpeedBoostDuration,
		false
	);

	Destroy();
}
