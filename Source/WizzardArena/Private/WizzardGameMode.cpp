// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardGameMode.h"

#include "EngineUtils.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "WaveManager.h"
#include "WizzardCharacter.h"
#include "WizzardPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WaveWidget.h"
#include "Components/AudioComponent.h"
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

	WaveMgr->OnAllWavesCompleted.AddDynamic(this, &AWizzardGameMode::HandleLastDefaultWave);
	WaveMgr->OnBossWavesCompleted.AddDynamic(this, &AWizzardGameMode::HandleBossDefeated);

	// Delay StartNextWave by 1 seconds
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [WaveMgr]()
	{
		WaveMgr->StartNextWave();
	}, 1.0f, false);

	if (LevelMusic)
	{
		LevelMusicComponent = UGameplayStatics::SpawnSound2D(this, LevelMusic, 1.f, 1.f, 0.f);
		if (LevelMusicComponent)
		{
			LevelMusicComponent->Play();
		}
	}
}

void AWizzardGameMode::HandleLastDefaultWave()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleLastDefaultWave"));

	// Set a timer to play the cutscene after 2 seconds
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			PlayWinSequence();
		},
		2.0f, // delay in seconds
		false // do not loop
	);
}

void AWizzardGameMode::PlayWinSequence()
{
	if (!WinSequence) return;

	FMovieSceneSequencePlaybackSettings PlaybackSettings;

	// Spawn the Level Sequence Actor
	ALevelSequenceActor* SeqActor = GetWorld()->SpawnActor<ALevelSequenceActor>();
	if (!SeqActor) return;

	// Create the sequence player
	ULevelSequencePlayer* SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		WinSequence,
		PlaybackSettings,
		SeqActor
	);

	if (SeqPlayer)
	{
		SeqPlayer->Play();
	}
}


void AWizzardGameMode::HandleBossDefeated()
{
	UE_LOG(LogTemp, Warning, TEXT("YOU WIN!"));

	// Disable input for the first player
	if (AWizzardPlayerController* PC = Cast<AWizzardPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->DisableInput(PC);
	}
	
	// Broadcast event — HUD will show endgame widget
	OnGameWon.Broadcast();
	
	// Lower background music
	if (LevelMusicComponent)
	{
		LevelMusicComponent->FadeOut(2.0f, 0.2f);
	}
	
	// Play victory music
	if (VictoryMusic)
	{
		UGameplayStatics::PlaySound2D(this, VictoryMusic);
	}
	
	// Rotate character
	if (AWizzardCharacter* Player = Cast<AWizzardCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->HandleWin();
	}
}