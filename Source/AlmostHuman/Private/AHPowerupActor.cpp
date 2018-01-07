// Fill out your copyright notice in the Description page of Project Settings.

#include "AHPowerupActor.h"


// Sets default values
AAHPowerupActor::AAHPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
}

// Called when the game starts or when spawned
void AAHPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAHPowerupActor::ActivatePowerup()
{
	OnActivated();

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &AAHPowerupActor::OnTickPowerup, PowerupInterval, true);
	}

	else
	{
		OnTickPowerup();
	}
}

void AAHPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTick();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}