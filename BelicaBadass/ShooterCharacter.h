// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USoundCue;
class AItem;
class AWeapon;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

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

	// Make sure camera isn't zoomed in when game starts
	void SetDefaultCameraView();

	// Called for forwards and backwards input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// Called via input to look side to side at a given rate
	void TurnAtRate(float Rate);

	// Called via input to look up and down at a given rate
	void LookUpAtRate(float Rate);

	// Starts a series of events related to firing the weapon
	void FireWeapon();

	// Plays the animation for firing the Weapon
	void PlayGunFireMontage();

	// Starts the line trace to determine direction of particles and impact points
	void SendBullet();

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

	// Sets bFireBullet to true and starts the crosshair mover timer
	void StartCrosshairBulletFire();

	// Set bFireBullet to false and ends the crosshair mover timer
	UFUNCTION()
	void FinishCrosshairBulletFire();

	// Called when the Fire Weapon button is pressed and released
	void FireButtonPressed();
	void FireButtonReleased();

	// Calls FireWeapon() and starts the timer for automatic gun fire
	void StartFireButtonTimer();

	// Ends the timer for automatic gun fire unless the fire weapon button is still pressed
	UFUNCTION()
	void AutoFireReset();

	// Line trace for Items under the crosshairs
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	// Trace for items if OverlappedItemCount > 0
	void TraceForItems();

	// Spawns the Weapon the character is holding when the game starts
	AWeapon* SpawnDefaultWeapon();

	// Takes a Weapon and attaches it to the mesh
	void EquipWeapon(AWeapon* WeaponToEquip);

	// Detach Weapon and let it fall to the ground
	void DropWeapon();

	// Starts the sequence that swaps the Weapon
	void EquipButtonPressed();

	// Drops EquippedWeapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

	// Initialize the AmmoMap with Ammo values
	void InitializeAmmoMap();

	// Check to make sure our Weapon has ammo
	bool WeaponHasAmmo();

	// Calls ReloadWeapon function
	void ReloadButtonPressed();

	// Starts a sequence of events that reloads EquippedWeapon
	void ReloadWeapon();

	// Called from blueprints to reset CombatState to unoccupied
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// Returns true if we have Ammo of the correct AmmoType
	bool CarryingAmmo();

	// Called from Animation Blueprint with GrabClip notifiy
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	// Called from Animation Blueprint with ReleaseClip notify
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	// Called when LeftShift button is pressed during gameplay
	void CrouchButtonPressed();

	virtual void Jump() override;

	// Interps capsule half height when moving between crouching and standing
	void InterpCapsuleHalfHeight(float DeltaTime);

	void TakeAim();
	void StopAiming();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called from blueprints to move the crosshairs during certain actions
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	// Adds or subtracts to or from OverlappedItemCount
	void IncrementOverlappedItemCount(int8 Amount);

	// Updates the camera smoothly when zooming and not zooming
	FVector GetCameraInterpLocation();

	// Determines how pickup Item is handled by the Character
	void GetPickupItem(AItem* Item);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
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

	/* True if we should trace every frame for Items */
	bool bShouldTraceForItems;

	/* Number of overlapped Items */
	int8 OverlappedItemCount;

	/* The Item we hit last frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	/* Currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	/* Set this in Blueprints for the default Weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/* The item currently hit by our trace in TraceForItems (could be null) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	/* Distance outward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance;

	/* Distance upward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation;

	/* Map to keep track of Ammo of the different ammo types  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	/* Starting amount of 9mm Ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo;

	/* Starting amount of AR Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	/* Combat State, can only fire or reload if Unoccupied*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	/* Montage for reload animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	/* Transform of the clip when we first grab it during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	/* Scene component to attach to the Character's hand during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	/* True when Character is crouching */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crouching, meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	/* Regular movement speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	/* Crouch movement speed*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	/* Current half height of the capsule */
	float CurrentCapsuleHalfHeight;

	/* Half height of the capsule when not crouching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;

	/* Half height of the capsule when crouching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;

	/* Ground friction while not crouching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction;

	/* Ground friction while crouching*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingGroundFriction;

	/* True when the player is holding the right mouse button */
	bool bAimingButtonPressed;

public:
	// Getters for private variables
	FORCEINLINE bool GetAiming() const { return bAiming; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
