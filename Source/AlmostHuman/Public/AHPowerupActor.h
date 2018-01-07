// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AHPowerupActor.generated.h"

UCLASS()
class ALMOSTHUMAN_API AAHPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAHPowerupActor();

protected:
	/* Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;

	/* Total times we apply the powerup effect*/
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	// Total number of ticks applied
	int32 TicksProcessed;

	UFUNCTION()
	void OnTickPowerup();

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

public:	
	void ActivatePowerup(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivated(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTick();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();
};
