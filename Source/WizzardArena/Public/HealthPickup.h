// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API AHealthPickup : public APickupItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float HealingAmount = 20.0f;
	
	void OnPickedUp(AWizzardCharacter* Player) override;
};
