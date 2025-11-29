// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPickup.h"

#include "WizzardCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASpeedPickup::OnPickedUp(AWizzardCharacter* Player)
{
	Super::OnPickedUp(Player);

	if (!Player) return;

	UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
	if (!MoveComp) return;

	const float OriginalSpeed = MoveComp->MaxWalkSpeed;

	MoveComp->MaxWalkSpeed = OriginalSpeed + SpeedBoostAmount;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[MoveComp, OriginalSpeed]()
		{
			MoveComp->MaxWalkSpeed = OriginalSpeed;
		},
		BoostDuration,
		false
	);

	Destroy();
}
