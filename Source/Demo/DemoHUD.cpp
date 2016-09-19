// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "DemoHUD.h"
#include "DemoCharacter.h"
#include "DemoUsableActor.h"
#include "DemoPlayerController.h"


ADemoHUD::ADemoHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDCenterDotObj(TEXT("/Game/UI/HUD/T_CenterDot_M"));

	CenterDotIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);
}


void ADemoHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCenterDot();
}


void ADemoHUD::DrawCenterDot()
{
	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;

	float CenterDotScale = 0.07f;

	ADemoPlayerController* PCOwner = Cast<ADemoPlayerController>(PlayerOwner);
	if (PCOwner)
	{
		ADemoCharacter* Pawn = Cast<ADemoCharacter>(PCOwner->GetPawn());
		if (Pawn /*&& Pawn->IsAlive()*/) // Disabled this check until Death & Respawn is implemented.
		{
			// Boost size when hovering over a switchable object.
			ADemoUsableActor* usable = Pawn->GetUsableInView();
			if (usable)
				CenterDotScale *= 1.5f;

			Canvas->SetDrawColor(255, 255, 255, 255);
			Canvas->DrawIcon(CenterDotIcon,
				CenterX - CenterDotIcon.UL*CenterDotScale / 2.0f,
				CenterY - CenterDotIcon.VL*CenterDotScale / 2.0f, CenterDotScale);
		}
	}
}


