// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveWidget.h"
#include "Components/TextBlock.h"

void UWaveWidget::SetWave(int32 WaveNumber)
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveNumber)));
	}
}

void UWaveWidget::SetEnemiesRemaining(int32 EnemiesLeft)
{
	if (EnemiesText)
	{
		EnemiesText->SetText(FText::FromString(FString::Printf(TEXT("Enemies Left: %d"), EnemiesLeft)));
	}
}