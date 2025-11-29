// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardGameMode.h"

#include "EngineUtils.h"
#include "WaveManager.h"
#include "WizzardCharacter.h"
#include "WizzardPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WaveWidget.h"
#include "Kismet/GameplayStatics.h"
#include "VT/LightmapVirtualTexture.h"

AWizzardGameMode::AWizzardGameMode()
{
	
}

void AWizzardGameMode::BeginPlay()
{
	Super::BeginPlay();

	AWaveManager* WaveMgr = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
	if (!WaveMgr)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveManager not found in level!"));
		return;
	}

	WaveMgr->OnAllWavesCompleted.AddDynamic(this, &AWizzardGameMode::HandleWinCondition);

	WaveHUDInstance = CreateWidget<UWaveWidget>(GetWorld(), WaveWidgetClass);
	if (!WaveHUDInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveWidgetClass is missing!"));
		return;
	}
	WaveHUDInstance->AddToViewport();
	WaveMgr->WaveWidget = WaveHUDInstance;
	WaveMgr->StartNextWave();

	if (LevelMusic)
	{
		UGameplayStatics::PlaySound2D(this, LevelMusic);
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
