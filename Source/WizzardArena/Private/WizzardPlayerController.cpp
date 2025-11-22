// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "WizzardCharacter.h"
#include "WizzardHUD.h"

void AWizzardPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	if (WizzardHUDClass)
	{
		WizzardHUD = CreateWidget<UWizzardHUD>(this, WizzardHUDClass);
		if (WizzardHUD)
		{
			WizzardHUD->AddToViewport();

			// Give reference to Character
			if (AWizzardCharacter* MyChar = Cast<AWizzardCharacter>(GetPawn()))
			{
				MyChar->SetHUDReference(WizzardHUD);
			}
		}
	}
}
