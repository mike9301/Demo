// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "Battery.h"

ABattery::ABattery(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemEnergy = 25;
	MeshComp->SetRenderCustomDepth(true);
}


void ABattery::OnUsed(APawn* InstigatorPawn)
{
	// Plays pickup sound from base class
	Super::OnUsed(InstigatorPawn);

	ADemoCharacter* Pawn = Cast<ADemoCharacter>(InstigatorPawn);
	if (Pawn)
	{
		Pawn->RecoverEnergy(ItemEnergy);
	}

	// Remove from level
	Destroy();
}



