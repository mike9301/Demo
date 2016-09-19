// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "DemoPlayerCameraManager.generated.h"
/**
*
*/
UCLASS()
class DEMO_API ADemoPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	ADemoPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	virtual void UpdateCamera(float DeltaTime) override;

public:

	/* default, hip fire FOV */
	float NormalFOV;

	/* aiming down sight / zoomed FOV */
	float TargetingFOV;

	float CurrentCrouchOffset;

	/* Maximum camera offset applied when crouch is initiated. Always lerps back to zero */
	float MaxCrouchOffsetZ;

	float CrouchLerpVelocity;

	bool bWasCrouched;

	/* Default relative Z offset of the player camera */
	float DefaultCameraOffsetZ;


};

