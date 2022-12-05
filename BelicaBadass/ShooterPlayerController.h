// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UUserWidget;

UCLASS()
class BELICABADASS_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	/* Reference to the Overall HUD Overlay Blueprint Class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUD_OverlayClass;

	/* Variable to hold the HUD Overlay widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUD_Overlay;
	
};
