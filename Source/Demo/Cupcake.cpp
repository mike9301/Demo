// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "Cupcake.h"


ACupcake::ACupcake(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemHealth = 25;
}


void ACupcake::OnUsed(APawn* InstigatorPawn)
{
	// Plays pickup sound from base class
	Super::OnUsed(InstigatorPawn);

	ADemoCharacter* Pawn = Cast<ADemoCharacter>(InstigatorPawn);
	if (Pawn)
	{
		Pawn->RecoverHealth(ItemHealth);
	}

	// Remove from level
	Destroy();
}
