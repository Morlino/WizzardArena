// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardHealthWidget.h"
#include "Components/ProgressBar.h"

void UWizzardHealthWidget::SetHealth(float Current, float Max)
{
	if (!HealthBar) return;

	HealthBar->SetPercent(Current / Max);
}
