// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "DemoHUD.generated.h"
/**
*
*/
UCLASS()
class DEMO_API ADemoHUD : public AHUD
{
	GENERATED_BODY()

		ADemoHUD(const FObjectInitializer& ObjectInitializer);

	FCanvasIcon CenterDotIcon;

	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	void DrawCenterDot();

public:
	/* An event hook to call HUD text events to display in the HUD. Blueprint HUD class must implement how to deal with this event. */
	UFUNCTION(BlueprintImplementableEvent, Category = "HUDEvents")
		void MessageReceived(const FString& TextMessage);


};
