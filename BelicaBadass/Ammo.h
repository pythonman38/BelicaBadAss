// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Ammo.generated.h"

UCLASS()
class BELICABADASS_API AAmmo : public AItem
{
	GENERATED_BODY()
	
public:
	AAmmo();

	virtual void Tick(float DeltaTime) override;

	virtual void EnableCustomDepth() override;

	virtual void DisableCustomDepth() override;

protected:
	virtual void BeginPlay() override;

	// Override of SetItemProperties to set AmmoMesh properties
	virtual void SetItemProperties(EItemState State) override;

	// Equips the Ammo to the Character when running over the Ammo
	UFUNCTION()
	void AmmoSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

private:
	/* Mesh for the Ammo pickup*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	/* Ammo type for the ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (ALlowPrivateAccess = "true"))
	EAmmoType AmmoType;

	/* The texture for the Ammo icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (ALlowPrivateAccess = "true"))
	UTexture2D* AmmoIconTexture;

	/* Overlap sphere for picking up the Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (ALlowPrivateAccess = "true"))
	USphereComponent* AmmoCollisionSphere;

public:
	// Getters for private variables
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
};
