// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(.7f),
	bFalling(false),
	Ammo(30),
	MagazineCapacity(30),
	WeaponType(EWeaponType::EWT_SubmachineGun),
	AmmoType(EAmmoType::EAT_9mm),
	ReloadMontageSection(FName("")),
	ClipBoneName(FName("")),
	bMovingClip(false),
	SlideDisplacement(0.f),
	SlideDisplacementTime(0.1f),
	bMovingSlide(false),
	MaxSlideDisplacement(4.f),
	bAutomatic(true)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KeepWeaponUpright();

	UpdateSlideDisplacement();
}

void AWeapon::KeepWeaponUpright()
{
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FString WeaponTablePath(TEXT("DataTable'/Game/_Game/DataTables/WeaponDataTable.WeaponDataTable'"));
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));
	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponDataRow = nullptr;
		switch (WeaponType)
		{
		case EWeaponType::EWT_SubmachineGun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SubmachineGun"), TEXT(""));
			break;
		case EWeaponType::EWT_AssualtRifle:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
			break;
		case EWeaponType::EWT_Pistol:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
			break;
		}

		if (WeaponDataRow)
		{
			AmmoType = WeaponDataRow->AmmoType;
			Ammo = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazineCapacity;
			SetPickupSound(WeaponDataRow->PickupSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetInventoryIcon(WeaponDataRow->InventoryIcon);
			SetAmmoIcon(WeaponDataRow->AmmoIcon);

			SetMaterialInstance(WeaponDataRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponDataRow->MaterialIndex);
			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimationBlueprint);
			CrosshairsMiddle = WeaponDataRow->CrosshairsMiddle;
			CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
			CrosshairsRight = WeaponDataRow->CrosshairsRight;
			CrosshairsTop = WeaponDataRow->CrosshairsTop;
			CrosshairsBottom = WeaponDataRow->CrosshairsBottom;
			AutoFireRate = WeaponDataRow->AutoFireRate;
			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			BoneToHide = WeaponDataRow->BoneToHide;
			bAutomatic = WeaponDataRow->bAutomatic;
		}

		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("FresnelColor"), GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());
			EnableGlowMaterial();
		}
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (BoneToHide != FName("None")) GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
}

void AWeapon::FinishMovingSlide()
{
	bMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if (SlideDisplacementCurve && bMovingSlide)
	{
		const float ElaspsedTime{ GetWorldTimerManager().GetTimerElapsed(SlideTimer) };
		const float CurveValue{ SlideDisplacementCurve->GetFloatValue(ElaspsedTime) };
		SlideDisplacement = CurveValue * MaxSlideDisplacement;
	}
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload more than MagazineCapacity!!!"));
	Ammo += Amount;
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MagazineCapacity;
}

void AWeapon::StartSlideTimer()
{
	bMovingSlide = true;
	GetWorldTimerManager().SetTimer(SlideTimer, this, &AWeapon::FinishMovingSlide, SlideDisplacementTime);
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	const FVector MeshForward{ GetItemMesh()->GetForwardVector() }, MeshRight{ GetItemMesh()->GetRightVector() };

	// Direction in which we throw the Weapon
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);
	float RandomRotation{ FMath::FRandRange(28.f, 32.f)};
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= 20'000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	// Start the ThrowWeaponTimer
	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

	EnableGlowMaterial();
}

void AWeapon::DecrementAmmo()
{
	(Ammo - 1 <= 0) ? Ammo = 0 : --Ammo;
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);

	StartPulseTimer();
}
