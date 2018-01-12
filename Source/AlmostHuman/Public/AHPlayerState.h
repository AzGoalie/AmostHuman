// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ALMOSTHUMAN_API AAHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddScore(float ScoreDelta);
	
};
