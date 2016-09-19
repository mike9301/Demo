// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Demo.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "DemoCharacter.h"
#include "DemoUsableActor.h"
#include "DemoHUD.h"

//////////////////////////////////////////////////////////////////////////
// ADemoCharacter

ADemoCharacter::ADemoCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoomComp->SocketOffset = FVector(0, 35, 0);
	CameraBoomComp->TargetOffset = FVector(0, 0, 55);
	CameraBoomComp->SetupAttachment(RootComponent);
	CameraBoomComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(CameraBoomComp, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
	MaxUseDistance = 300;
	bHasNewFocus = true;
	TargetingSpeedModifier = 0.5f;
	Health = 100;
	Energy = 100;
	InitialEnergy = 75;
	InitialHealth = 75;
	Level = 1;
	Xp = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADemoCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ADemoCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADemoCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADemoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADemoCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADemoCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ADemoCharacter::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ADemoCharacter::StopCrouching);

	PlayerInputComponent->BindAction("Use", IE_Released, this, &ADemoCharacter::Use);

	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ADemoCharacter::Targeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ADemoCharacter::StopTargeting);
}


void ADemoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADemoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ADemoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ADemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADemoCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADemoCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADemoCharacter::Jump()
{
	if (bIsCrouched == false)
	{
		bPressedJump = true;
		SetIsJumping(true);
	}
}

void ADemoCharacter::StopJumping()
{
	bPressedJump = false;
	SetIsJumping(false);

	CheckResetJumpCount();
}

void ADemoCharacter::SetIsJumping(bool NewJumping)
{
	if (Role < ROLE_Authority)
	{
		ServerSetIsJumping(NewJumping);
	}
	else
	{
		bIsJumping = NewJumping;
	}
}

void ADemoCharacter::ServerSetIsJumping_Implementation(bool NewJumping)
{
	SetIsJumping(NewJumping);
}

bool ADemoCharacter::ServerSetIsJumping_Validate(bool NewJumping)
{
	return true;
}

bool ADemoCharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

void ADemoCharacter::Crouching()
{
	Crouch();
}

void ADemoCharacter::StopCrouching()
{
	UnCrouch();
}

void ADemoCharacter::Targeting()
{
	SetIsTargeting(true);
}

void ADemoCharacter::StopTargeting()
{
	SetIsTargeting(false);
}

void ADemoCharacter::SetIsTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;

	if (Role < ROLE_Authority)
	{
		ServerSetIsTargeting(NewTargeting);
	}
}

void ADemoCharacter::ServerSetIsTargeting_Implementation(bool NewTargeting)
{
	SetIsTargeting(NewTargeting);
}


bool ADemoCharacter::ServerSetIsTargeting_Validate(bool NewTargeting)
{
	return true;
}


bool ADemoCharacter::IsTargeting() const
{
	return bIsTargeting;
}

float ADemoCharacter::GetTargetingSpeedModifier() const
{
	return TargetingSpeedModifier;
}

FRotator ADemoCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void ADemoCharacter::Use()
{
	if (Role == ROLE_Authority)
	{
		ADemoUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			Usable->OnUsed(this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ADemoCharacter::ServerUse_Implementation()
{
	Use();
}


bool ADemoCharacter::ServerUse_Validate()
{
	return true;
}

ADemoUsableActor* ADemoCharacter::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return NULL;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<ADemoUsableActor>(Hit.GetActor());
}

void ADemoCharacter::RecoverHealth(float ItemHealth)
{
	Health = FMath::Clamp(Health + ItemHealth, 0.0f, GetMaxHealth());

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		ADemoHUD* MyHUD = Cast<ADemoHUD>(PC->GetHUD());
		if (MyHUD)
		{
			MyHUD->MessageReceived("Health recovery!");
		}
	}
}

void ADemoCharacter::RecoverEnergy(float ItemEnergy)
{
	Health = FMath::Clamp(Health + ItemEnergy, 0.0f, GetMaxEnergy());

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		ADemoHUD* MyHUD = Cast<ADemoHUD>(PC->GetHUD());
		if (MyHUD)
		{
			MyHUD->MessageReceived("Energy recovery!");
		}
	}
}

float ADemoCharacter::GetHealth() const
{
	return Health;
}


float ADemoCharacter::GetEnergy() const
{
	return Energy;
}

float ADemoCharacter::GetMaxHealth() const
{
	return InitialHealth + Level * 25;
}


float ADemoCharacter::GetMaxEnergy() const
{
	return InitialEnergy + Level * 25;
}

void ADemoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		// Set a timer to increment hunger every interval
		/*FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASCharacter::IncrementHunger, IncrementHungerInterval, true);*/

		//SpawnDefaultInventory();
	}
}

void ADemoCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Value is already updated locally, skip in replication step
	DOREPLIFETIME_CONDITION(ADemoCharacter, bIsTargeting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ADemoCharacter, bIsJumping, COND_SkipOwner);

	// Replicate to every client, no special condition required
	DOREPLIFETIME(ADemoCharacter, Health);
	DOREPLIFETIME(ADemoCharacter, Energy);
	DOREPLIFETIME(ADemoCharacter, Level);
}

void ADemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && Controller->IsLocalController())
	{
		ADemoUsableActor* Usable = GetUsableInView();

		// End Focus
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus
		FocusedUsableActor = Usable;

		// Start Focus.
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
	}
}