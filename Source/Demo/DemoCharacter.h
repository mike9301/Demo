// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DemoCharacter.generated.h"

UCLASS(config = Game)
class ADemoCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoomComp;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;
public:
	ADemoCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsTargeting() const;

	virtual void Use();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	class ADemoUsableActor* GetUsableInView();

	float GetTargetingSpeedModifier() const;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	void RecoverHealth(float ItemHealth);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	void RecoverEnergy(float Energy);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetEnergy() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetMaxEnergy() const;
	
	class ADemoUsableActor* FocusedUsableActor;

	FORCEINLINE UCameraComponent* GetCameraComponent()
	{
		return CameraComp;
	}

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<class ADemoWeapon>> DefaultInventoryClasses;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Jump();

	void StopJumping();

	UPROPERTY(Transient, Replicated)
	bool bIsJumping;

	void SetIsJumping(bool NewJumping);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsJumping(bool NewJumping);

	void Crouching();

	void StopCrouching();


	void Targeting();

	void StopTargeting();

	void SetIsTargeting(bool NewTargeting);

	UPROPERTY(Transient, Replicated)
	bool bIsTargeting;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsTargeting(bool NewTargeting);

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	float Energy;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float InitialHealth;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float InitialEnergy;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	int Level;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float Xp;

	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	bool bHasNewFocus;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoomComp; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

	virtual void Tick(float DeltaSeconds) override;

};

