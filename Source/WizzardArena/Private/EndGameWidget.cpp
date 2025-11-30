// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameWidget.h"
#include "Components/TextBlock.h"

void UEndGameWidget::ShowWin()
{
	if (EndGameText)
	{
		EndGameText->SetText(FText::FromString("YOU WIN!"));
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UEndGameWidget::ShowLose()
{
	if (EndGameText)
	{
		EndGameText->SetText(FText::FromString("YOU LOSE!"));
		SetVisibility(ESlateVisibility::Visible);
	}
}
