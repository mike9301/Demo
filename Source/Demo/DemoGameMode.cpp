// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Demo.h"
#include "DemoGameMode.h"
#include "DemoCharacter.h"
#include "DemoPlayerController.h"
#include "DemoHUD.h"

ADemoGameMode::ADemoGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/AnimStarterPack/Ue4ASP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = ADemoPlayerController::StaticClass();
	HUDClass = ADemoHUD::StaticClass();
}