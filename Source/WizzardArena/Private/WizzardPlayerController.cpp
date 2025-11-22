// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AWizzardPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
}
