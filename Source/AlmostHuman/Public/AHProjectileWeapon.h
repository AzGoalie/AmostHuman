// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHWeapon.h"
#include "AHProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ALMOSTHUMAN_API AAHProjectileWeapon : public AAHWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void Fire() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;
};
