// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "WaveManager.h"
#include "WizzardCharacter.h"
#include "WizzardGameMode.h"
#include "WizzardHUD.h"
#include "Kismet/GameplayStatics.h"

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

			if (AWaveManager* WM = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(this, AWaveManager::StaticClass())))
			{
				WM->SetHUDReference(WizzardHUD);
			}

			WizzardHUD->InitializeWidgets();
		}
	}

	if (AWizzardGameMode* GM = Cast<AWizzardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (WizzardHUD) // the instance you created above
		{
			GM->OnGameWon.AddDynamic(WizzardHUD, &UWizzardHUD::ShowWin);
			GM->OnGameLost.AddDynamic(WizzardHUD, &UWizzardHUD::ShowLose);
		}
	}
}

void AWizzardPlayerController::ClientShowWinScreen()
{

}
