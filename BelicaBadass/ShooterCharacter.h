// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USoundCue;

UCLASS()
class BELICABADASS_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called for forwards and backwards input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// Called via input to look side to side at a given rate
	void TurnAtRate(float Rate);

	// Called via input to look up and down at a given rate
	void LookUpAtRate(float Rate);

	// Called when the Fire Weapon button is pressed
	void FireWeapon();

	// Returns true when the line trace hits an object
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	// Called when Aiming button is pressed
	void AimingButtonPressed();

	// Called when Aiming button is released
	void AimingButtonReleased();

	// Interp to appropriate field of view when changing between aiming and NOT aiming
	void CameraInterpZoom(float DeltaTime);

	// Spreads the crosshairs based on movement and activity of the Character
	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireButtonTimer();

	UFUNCTION()
	void AutoFireReset();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

private:
	/* Camera boom positioning the camera behind the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/* Camera that follows the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/* Base turn rate in degrees per second */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/* Base look up rate in degrees per second */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/* Randomized sound played when the Fire Weapon button is pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	/* Flash that spawns at BarrelSocket when Weapon is fired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	/* Montage for firing the Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage;

	/* Particles spawned upon bullet impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	/* Smoke trail for bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	/* True when the Character is Aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	/* Default camera field of view value for when Character is NOT aiming */
	float CameraDefaultFOV;

	/* Camera field of view value when the Character IS aiming */
	float CameraZoomedFOV;

	/* Camera field of view value while interpolating */
	float CameraCurrentFOV;

	/* Interp speed for zooming when aiming */
	float ZoomInterpSpeed;

	/* Crosshair multiplier value based on the movement and actions of the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	/* Crosshair spread values based on speed, in-air status, aiminging and shooting, respectively */
	float CrosshairVelocityFactor, CrosshairInAirFactor, CrosshairAimFactor, CrosshairShootingFactor;

	/* Variables that handle the shooting timer */
	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	/* True when the fire weapon button is pressed */
	bool bFireButtonPressed;

	/* True when the Character is able to fire the weapon */
	bool bShouldFire;

	/* Rate of automatic gun fire */
	float AutomaticFireRate;

	/* Sets a timer*/
	FTimerHandle AutoFireTimer;

public:
	// Getters for private variables
	FORCEINLINE bool GetAiming() const { return bAiming; }
};
