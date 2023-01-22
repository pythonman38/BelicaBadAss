// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

class UBoxComponent;
class UWidgetComponent;
class USphereComponent;
class AShooterCharacter;
class UCurveFloat;
class USoundCue;
class UCurveVector;
class UDataTable;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIS_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;
};

UCLASS()
class BELICABADASS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets the ActiveStars array of bools based on rarity
	void SetActiveStars();

	// Sets properties of the Item's components based on State
	virtual void SetItemProperties(EItemState State);

	// Called when ItemInterpTimer is finished
	void FinishInterping();

	// Handle item interpolation when in the EquipInterping state
	void ItemInterp(float DeltaTime);

	// Get Interp location based on the item type
	FVector GetInterpLocation();

	void PlayPickupSound();

	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void ResetPulseTimer();

	void UpdatePulse();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetItemState(EItemState State);

	// Called from the AShooterCharacter class 
	void StartItemCurve(AShooterCharacter* Char);

	void PlayEquipSound();

	virtual void EnableCustomDepth();

	virtual void DisableCustomDepth();

	void DisableGlowMaterial();

	void StartPulseTimer();

private:
	/* Skeletal mesh for the Item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	/* Line trace collides with box to show HUD widgets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	/* Popup widget for when the player looks at the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PickupWidget;

	/* Enables item tracing when overlapped */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AreaSphere;

	/* The name which appears on the PickupWidget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	/* Item count (ammo, etc) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	/* Item rarity - determines number of stars in PickupWidget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	/* Array used to turn on stars based on ItemRarity value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	/* State of the Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	/* The curve asset to use for the item's Z location when interping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemZCurve;

	/* Starting location when interping begins */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	/* Target interp location in front of the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	/* True when interpolating */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterping;

	/* Plays when we start interpolating */
	FTimerHandle ItemInterpTimer;

	/* Duration of the curve and timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	/* Get a reference pointer to the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* Character;

	/* X and Y for the Item while interping in the EquipInterp State */
	float ItemInterpX, ItemInterpY;

	/* Initial Yaw offsets between the camera and the interping item */
	float InterpInitialYawOffset;

	/* Curve used to scale the item when interpolating */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	/* Sound played when Item is picked up*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* PickupSound;

	/* Sound played when the Item is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;

	/* Enum for the type of Item the Character can pick up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	/* Index of the interp location this item is interpolating to */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocIndex;

	/* Index for the material we'd like to change at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;

	/* Dynamic Instance that we an change at runtime */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	/* Material Instance used with the Dynamic Material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	/* True when Custom Depth on Items can be enabled */
	bool bCanChangeCustomDepth; 

	/* Curve to drive the dynamic material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* PulseCurve;

	FTimerHandle PulseTimer;

	/* Time for the Pulsetimer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelReflectFraction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* InterpPulseCurve;

	/* Weapon Icon for this Item in the Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;

	/* Ammo Icon for this Item in the INventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconAmmo;

	/* Slot in the Inventory array */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	/* True when the Character's Inventory is full */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bCharacterInventoryFull;

	/* Item Rarity data table */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemRarityDataTable;

	/* Color in the Item's glow material */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	/* Light color in the pickup widget  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;

	/* Dark color in the pickup widget  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;

	/* Number of starts in the pickup widget  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	/* Background color for this Item in the Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

public:
	// Getters for private variables
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE int32 GetMaterialIndex() const { return MaterialIndex; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance; }
	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }

	// Setters for private variables
	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { IconAmmo = Icon; }
	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInventoryFull = bFull; }
	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) { DynamicMaterialInstance = Instance; }
	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }
	FORCEINLINE void SetInventoryIcon(UTexture2D* Icon) { IconItem = Icon; }
	FORCEINLINE void SetMaterialIndex(int32 Index) { MaterialIndex = Index; }
	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }
	FORCEINLINE void SetPickupSound(USoundCue* Sound) { PickupSound = Sound; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }
};
