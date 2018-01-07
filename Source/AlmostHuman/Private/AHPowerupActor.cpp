// Fill out your copyright notice in the Description page of Project Settings.

#include "AHPowerupActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAHPowerupActor::AAHPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;

	bIsPowerupActive = false;

	SetReplicates(true);
}

void AAHPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void AAHPowerupActor::ActivatePowerup(AActor* ActivateFor)
{
	OnActivated(ActivateFor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();

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

		bIsPowerupActive = false;
		OnRep_PowerupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void AAHPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAHPowerupActor, bIsPowerupActive);
}