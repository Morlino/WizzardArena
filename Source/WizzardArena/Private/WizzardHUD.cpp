// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardHUD.h"

#include "EndGameWidget.h"
#include "PowerUpsWidget.h"
#include "WaveManager.h"
#include "WaveWidget.h"
#include "WizzardHealthWidget.h"
#include "Kismet/GameplayStatics.h"

void UWizzardHUD::InitializeWidgets()
{
	if (WaveWidget)
	{
		WaveWidget->SetWave(0);
		WaveWidget->SetEnemiesRemaining(0);
	}

	if (PowerUpsWidget)
	{
		PowerUpsWidget->UpdateShieldHits(0);
		PowerUpsWidget->UpdateRapidFireBuffs(0);
		PowerUpsWidget->UpdateSpeedBuffs(0);
	}

	if (EndGameWidget)
	{
		EndGameWidget->SetVisibility(ESlateVisibility::Hidden);	
	}
}

void UWizzardHUD::SetHealth(float Current, float Max)
{
	HealthWidget->SetHealth(Current, Max);
}

void UWizzardHUD::SetShieldHits(int32 Hits)
{
	if (PowerUpsWidget)
		PowerUpsWidget->UpdateShieldHits(Hits);
}

void UWizzardHUD::SetRapidFireBuffs(int32 Count)
{
	if (PowerUpsWidget)
		PowerUpsWidget->UpdateRapidFireBuffs(Count);
}

void UWizzardHUD::SetSpeedBuffs(int32 Count)
{
	if (PowerUpsWidget)
		PowerUpsWidget->UpdateSpeedBuffs(Count);
}

void UWizzardHUD::SetWave(int32 WaveNumber)
{
	if (WaveWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD Wave %d"), WaveNumber);
		WaveWidget->SetWave(WaveNumber);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD Wave NONE"));
	}
}

void UWizzardHUD::SetEnemiesRemaining(int32 EnemiesLeft)
{
	if (WaveWidget)
		WaveWidget->SetEnemiesRemaining(EnemiesLeft);
}

void UWizzardHUD::ShowWin()
{
	if (EndGameWidget)
	{
		EndGameWidget->ShowWin();
	}
}

void UWizzardHUD::ShowLose()
{
	if (EndGameWidget)
	{
		EndGameWidget->ShowLose();
	}
}