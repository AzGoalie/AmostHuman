// Fill out your copyright notice in the Description page of Project Settings.

#include "AHGameMode.h"
#include "TimerManager.h"
#include "AHHealthComponent.h"

AAHGameMode::AAHGameMode()
{
	TimeBetweenWaves = 2.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AAHGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AAHGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AAHGameMode::StartWave, TimeBetweenWaves, false);
}

void AAHGameMode::StartWave()
{
	WaveCount++;
	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AAHGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}

void AAHGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
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
	bool bisPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (NumberOfBotsToSpawn > 0 || bisPreparingForWave)
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
		PrepareForNextWave();
	}
}

void AAHGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}