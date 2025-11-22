// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardHUD.h"

#include "WizzardHealthWidget.h"

void UWizzardHUD::SetHealth(float Current, float Max)
{
	HealthWidget->SetHealth(Current, Max);
}
