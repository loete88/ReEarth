// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence\Public\LevelSequencePlayer.h"
#include "SequenceInfoStruct.generated.h"

USTRUCT(BlueprintType)
struct FSequenceInfoStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULevelSequence* LevelSequence;
	class ULevelSequencePlayer* LevelSequencePlayer;	
	bool isPlay;
};