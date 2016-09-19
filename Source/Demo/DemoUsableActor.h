// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DemoUsableActor.generated.h"

UCLASS()
class DEMO_API ADemoUsableActor : public AActor
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComp;

public:

	/* Player is looking at */
	virtual void OnBeginFocus();

	/* Player is no longer looking at */
	virtual void OnEndFocus();

	/* Called when player interacts with object */
	virtual void OnUsed(APawn* InstigatorPawn);



};
