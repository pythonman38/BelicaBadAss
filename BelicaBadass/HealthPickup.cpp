// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

AHealthPickup::AHealthPickup() :
	HealingAmount(20.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthPickupMesh"));
	SetRootComponent(HealthPickupMesh);

	HealthCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HealthCollision"));
	HealthCollisionSphere->SetupAttachment(GetRootComponent());
}

void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	HealthCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::HealthSphereOverlap);
}

void AHealthPickup::HealthSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto OverlappedCharacter = Cast<AShooterCharacter>(OtherActor);
		if (OverlappedCharacter && HealthPickupSound)
		{
			if (OverlappedCharacter->GetHealth() + HealingAmount > OverlappedCharacter->GetMaxHealth())
			{
				OverlappedCharacter->SetHealth(OverlappedCharacter->GetMaxHealth());
			}
			else OverlappedCharacter->SetHealth(OverlappedCharacter->GetHealth() + HealingAmount);

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HealthPickupSound, GetActorLocation());
		}

		Destroy();
	}
}
