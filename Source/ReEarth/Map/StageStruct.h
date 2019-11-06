// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence\Public\LevelSequencePlayer.h"
#include "StageStruct.generated.h"

USTRUCT(BlueprintType)
struct FStageStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULevelSequence* LevelSequence;
};