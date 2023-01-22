// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

class USoundCue;
class UWidgetComponent;

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaterialIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;
};

/**
 * 
 */
UCLASS()
class BELICABADASS_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

	// Adds an impulse to the Weapon
	void ThrowWeapon();

	// called from Character class when firing Weapon
	void DecrementAmmo();

	// The amount to load in the clip depending on how much Ammo it already has
	void ReloadAmmo(int32 Amount);

	bool ClipIsFull();

	void StartSlideTimer();

protected:
	// Called when Weapon
	void StopFalling();

	// Keeps Weapon upright when thrown to the ground
	void KeepWeaponUpright();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void FinishMovingSlide();

	void UpdateSlideDisplacement();

private:
	/* Variables for handling the ThrowWeaponTimer */
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/* Ammo count for this Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	/* Maximum Ammo that our Weapon can hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	/* Type of Weapon equipped by the Character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	/* The type of Ammo for this Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	/* FName for the Reload Montage Section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	/* True when moving the clip while reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	/* Name for the clip bone of the EquippedWeapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	/* Data table for Weapon properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

	/* Placeholder for clearing out the Material Index of the previously equipped Weapon */
	int32 PreviousMaterialIndex;

	/* Middle crosshair property of the currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsMiddle;

	/* Left crosshair property of the currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsLeft;

	/* Right crosshair property of the currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsRight;

	/* Bottom crosshair property of the currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsBottom;

	/* Top crosshair property of the currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsTop;

	/* The speed at which automatic gun fire happens */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;

	/* Partice system spawned at the BarrelSocket of the Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	/* Sound played with the Weapon is fired */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	/* Used on the Pistol to hide the Gun Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	FName BoneToHide;

	/* Amount that the slide is pushed back during Pistol fire */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float SlideDisplacement;

	/* Curve for the slide displacement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlideDisplacementCurve;

	/* Timer handle for updating SlideDisplacement */
	FTimerHandle SlideTimer;

	/* Time for displacing the slide during pistol fire */
	float SlideDisplacementTime;

	/* True when moving the Pistol slide */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	bool bMovingSlide;

	/* Max distance for the slide on the Pistol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float MaxSlideDisplacement;

	/* True when the Weapon has automatic gun fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;

public:
	// Getters for private variables
	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }

	// Setters for private variables
	FORCEINLINE void SetClipBoneName(FName Name) { ClipBoneName = Name; }
	FORCEINLINE void SetReloadMontageSection(FName Section) { ReloadMontageSection = Section; }
	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
};
