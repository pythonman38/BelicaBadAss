// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),
	EOS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class BELICABADASS_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:
	// Handle turning in place variables
	void TurnInPlace();

	// Handle calculations for leaning while running
	void Lean(float DeltaTime);

private:
	/* Get a referenc the ShooterCharacter */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	/* The speed of the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	/* True when the Character is in the air (from jumping or falling) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/* True when the Character is running or not standing still */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	/* Offset yaw used for straffing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	/* Offset yaw the frame before we stopped moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	/* True when the Character is aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	/* Rotation of the Character this frame */
	FRotator CharacterRotation;

	/* Rotation of the Character last frame */
	FRotator CharacterRotationLastFrame;

	/* Yaw of the Character this frame for turning in place */
	float TIP_CharacterYaw;

	/* Yaw of the Character the previous frame for turning in place */
	float TIP_CharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	/* Rotation curve value this frame */
	float RotationCurve;

	/* Rotation curve value last frame */
	float RotationCuveLastFame;

	/* The pitch of the aim rotation, used for Aim Offset */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
	float Pitch;

	/* True when the Character is reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	/* Offset state - used to determine which aim offset to use */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;

	/* Yaw delta used for leaning in the running blendspace */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	/* True when the Character is crouching */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crouching, meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	/* Change the recoil weight based on turning in place and aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float RecoilWeight;

	/* True when turning in place */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crouching, meta = (AllowPrivateAccess = "true"))
	bool bTurningInPlace;

	/* True when the Character is changing Weapons in Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crouching, meta = (AllowPrivateAccess = "true"))
	bool bEquipping;
};
