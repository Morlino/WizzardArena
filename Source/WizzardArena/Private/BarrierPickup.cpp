// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrierPickup.h"
#include "WizzardCharacter.h"
#include "Kismet/GameplayStatics.h"

void ABarrierPickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);

	if (!Player) return;
	
	Player->ActivateShield(1);
	UGameplayStatics::PlaySound2D(this, PickUpSound);
	Destroy();
}
