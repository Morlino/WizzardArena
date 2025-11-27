// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardGameMode.h"

#include "WaveManager.h"
#include "WizzardCharacter.h"
#include "WizzardPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "VT/LightmapVirtualTexture.h"

AWizzardGameMode::AWizzardGameMode()
{
	
}

void AWizzardGameMode::BeginPlay()
{
	Super::BeginPlay();

	AWaveManager* WaveMgr = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));

	if (WaveMgr)
	{
		WaveMgr->OnAllWavesCompleted.AddDynamic(this, &AWizzardGameMode::HandleWinCondition);
	}
}

void AWizzardGameMode::HandleWinCondition()
{
	UE_LOG(LogTemp, Warning, TEXT("YOU WIN!"));

	AWizzardPlayerController* PC = Cast<AWizzardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->DisableInput(PC);
		// PC->ClientShowWinScreen();
	}

	UGameplayStatics::PlaySound2D(this, VictoryMusic);

	if (AWizzardCharacter* Player = Cast<AWizzardCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->HandleWin();
	}
}
