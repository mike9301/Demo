// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "DemoPlayerController.h"
#include "DemoPlayerCameraManager.h"

ADemoPlayerController::ADemoPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ADemoPlayerCameraManager::StaticClass();
}


