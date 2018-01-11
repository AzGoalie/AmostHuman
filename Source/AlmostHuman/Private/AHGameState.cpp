// Fill out your copyright notice in the Description page of Project Settings.

#include "AHGameState.h"
#include "net/UnrealNetwork.h"


void AAHGameState::SetWaveState(EWaveState NewState)
{
	if (Role == ROLE_Authority)
	{
		EWaveState OldState = WaveState;
		WaveState = NewState;
		OnRep_WaveState(OldState);

	}
}

void AAHGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(OldState, WaveState);
}

void AAHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAHGameState, WaveState);
}