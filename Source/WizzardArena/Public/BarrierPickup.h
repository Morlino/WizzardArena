// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "BarrierPickup.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API ABarrierPickup : public APickupItem
{
	GENERATED_BODY()

public:
	void OnPickedUp(AWizzardCharacter* Player) override;
};
