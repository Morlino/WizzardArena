// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WizzardHUD.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UWizzardHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UWizzardHealthWidget* HealthWidget;

	void SetHealth(float Current, float Max);
};
