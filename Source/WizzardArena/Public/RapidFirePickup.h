// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "RapidFirePickup.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API ARapidFirePickup : public APickupItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CastSpeedBoostMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CastSpeedBoostDuration = 5.0f;
	
	void OnPickedUp(AWizzardCharacter* Player) override;
};
