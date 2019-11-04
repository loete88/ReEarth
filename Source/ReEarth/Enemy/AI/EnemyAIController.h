// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName IsAttackEndKey;
	static const FName IsMovePatrolLocEndKey;
	static const FName IsDestinationActorEndKey;
	static const FName IsVisibleAttackTargetKey;

	UFUNCTION(BlueprintCallable)
	void IsAttackEnd(bool Value);

	UFUNCTION(BlueprintCallable)
	void IsMovePatrolLocEnd(bool Value);

	UFUNCTION(BlueprintCallable)
	void IsDestinationActorEnd(bool Value);
	
protected:

private:
	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
};
