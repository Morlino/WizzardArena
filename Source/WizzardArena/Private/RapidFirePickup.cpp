// Fill out your copyright notice in the Description page of Project Settings.


#include "RapidFirePickup.h"
#include "WizzardCharacter.h"

void ARapidFirePickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);
	if (!Player) return;

	Player->ApplyRapidFire(CastSpeedBoostMultiplier);

	FTimerHandle TimerHandle;
	Player->GetWorldTimerManager().SetTimer(
		TimerHandle,
		[Player]()
		{
			Player->RemoveRapidFire();
		},
		CastSpeedBoostDuration,
		false
	);

	Destroy();
}
