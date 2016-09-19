// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "DemoPickupActor.h"


// Sets default values
ADemoPickupActor::ADemoPickupActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void ADemoPickupActor::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetSimulatePhysics(true);
}


