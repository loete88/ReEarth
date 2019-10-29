// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/VRHand/PlayerPawn.h"
#include "HandAnimation.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UHandAnimation : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EVRHandState VRHandState;
};
