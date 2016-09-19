// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DemoUsableActor.h"
#include "DemoCharacter.h"
#include "DemoPickupActor.generated.h"

UCLASS()
class DEMO_API ADemoPickupActor : public ADemoUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADemoPickupActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
