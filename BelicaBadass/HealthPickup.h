// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class BELICABADASS_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:
	AHealthPickup();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// Increases Character's health when overlapped
	UFUNCTION()
	void HealthSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/* Mesh for the Health pickup Item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Pickup", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HealthPickupMesh;

	/* Overlap sphere for the Health pickup Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Pickup", meta = (ALlowPrivateAccess = "true"))
	USphereComponent* HealthCollisionSphere;

	float HealingAmount;

	/* Sound played with Health pickup Item is collected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Pickup", meta = (AllowPrivateAccess = "true"))
	USoundCue* HealthPickupSound;
};
