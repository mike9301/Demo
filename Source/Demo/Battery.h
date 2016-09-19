// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DemoPickupActor.h"
#include "Battery.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API ABattery : public ADemoPickupActor
{
	GENERATED_BODY()

	ABattery(const FObjectInitializer& ObjectInitializer);

protected:

	void OnUsed(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	float ItemEnergy;
	
	
	
	
};
