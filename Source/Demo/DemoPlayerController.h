// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DemoPlayerController.generated.h"

/**
*
*/
UCLASS()
class DEMO_API ADemoPlayerController : public APlayerController
{
	GENERATED_BODY()

		ADemoPlayerController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		bool bRespawnImmediately;


};
