// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "WizzardCharacter.h"
#include "Kismet/GameplayStatics.h"

void AHealthPickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);

	if (Player)
	{
		if (Player->GetCurrentHealth() != Player->GetMaxHealth())
		{
			Player->Heal(HealingAmount);
			UGameplayStatics::PlaySound2D(this, PickUpSound);
			Destroy();
		}
	}
}
