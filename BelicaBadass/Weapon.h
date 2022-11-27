// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

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

protected:
	// Called when Weapon
	void StopFalling();

	// Keeps Weapon upright when thrown to the ground
	void KeepWeaponUpright();


private:
	/* Variables for handling the ThrowWeaponTimer */
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;
};
