// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class BELICABADASS_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	
	virtual void OnPossess(APawn* InPawn) override;

private:
	/* Blackboard comonent for this Enemy */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	/* Behavior Tree comonent for this Enemy */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

public:
	// Getters for private variables
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
