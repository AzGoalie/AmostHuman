// Fill out your copyright notice in the Description page of Project Settings.

#include "AHGameMode.h"
#include "TimerManager.h"
#include "AHHealthComponent.h"
#include "AHGameState.h"
#include "AHPlayerState.h"

AAHGameMode::AAHGameMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = AAHGameState::StaticClass();
	PlayerStateClass = AAHPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AAHGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AAHGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);
	// @TODO: Finish up the match, present 'game over' to players
	UE_LOG(LogTemp, Log, TEXT("GAME OVER! All players died!"));
}

void AAHGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AAHGameMode::StartWave, TimeBetweenWaves, false);
	SetWaveState(EWaveState::WaitingToStart);
}

void AAHGameMode::StartWave()
{
	WaveCount++;
	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AAHGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
	SetWaveState(EWaveState::WaveInProgress);
}

void AAHGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}

void AAHGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;
	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void AAHGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (NumberOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UAHHealthComponent* HealthComp = Cast<UAHHealthComponent>(TestPawn->GetComponentByClass(UAHHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void AAHGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UAHHealthComponent* HealthComp = Cast<UAHHealthComponent>(MyPawn->GetComponentByClass(UAHHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				// A player is still alive
				return;
			}
		}
	}

	// No player alive
	GameOver();
}

void AAHGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void AAHGameMode::SetWaveState(EWaveState NewState)
{
	AAHGameState* GS = GetGameState<AAHGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}