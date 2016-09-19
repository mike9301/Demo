// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "DemoUsableActor.h"


// Sets default values
ADemoUsableActor::ADemoUsableActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}


void ADemoUsableActor::OnUsed(APawn* InstigatorPawn)
{
	// Nothing to do here...
}


void ADemoUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}


void ADemoUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}


