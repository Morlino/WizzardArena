// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardHUD.h"
#include "PowerUpsWidget.h"
#include "WaveManager.h"
#include "WizzardHealthWidget.h"
#include "Kismet/GameplayStatics.h"

void UWizzardHUD::InitializeWidgets()
{
	// Pass WaveWidget to WaveManager
	if (AWaveManager* WaveMgr = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass())))
	{
		WaveMgr->WaveWidget = WaveWidget;
	}

	if (PowerUpsWidget)
	{
		PowerUpsWidget->UpdateShieldHits(0);
		PowerUpsWidget->UpdateRapidFireBuffs(0);
		PowerUpsWidget->UpdateSpeedBuffs(0);
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
