// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "SpeedPickup.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API ASpeedPickup : public APickupItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|Boost")
	float SpeedBoostAmount = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup|Boost")
	float BoostDuration = 5.f;

	void OnPickedUp(AWizzardCharacter* Player) override;
};
