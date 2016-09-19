// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DemoPickupActor.h"
#include "Cupcake.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API ACupcake : public ADemoPickupActor
{
	GENERATED_BODY()

	ACupcake(const FObjectInitializer& ObjectInitializer);
	
protected:

	void OnUsed(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	float ItemHealth;
	
	
};
