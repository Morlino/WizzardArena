// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "WizzardPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API AWizzardPlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UWizzardHUD> WizzardHUDClass;
	
	UPROPERTY()
	class UWizzardHUD* WizzardHUD;
};
