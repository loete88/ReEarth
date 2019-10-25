// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/PlayerRobot.h"
#include "PlayerRobotAnimation.generated.h"



/**
 * 
 */
UCLASS()
class REEARTH_API UPlayerRobotAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerRobot *	PlayerRobot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float			Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float			LeftAimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float			LeftAimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float			RightAimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float			RightAimPitch;
};
