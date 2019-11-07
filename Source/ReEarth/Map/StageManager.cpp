// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "Kismet\KismetSystemLibrary.h"
#include "MovieScene\Public\MovieSceneSequencePlayer.h"
#include "TimeManagement\Public\TimeManagementBlueprintLibrary.h"

// Sets default values
AStageManager::AStageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStageManager::BeginPlay()
{
	Super::BeginPlay();

	int a = 0;
	int b = 0;

	for (auto ind : StageIndex)
	{
		for (auto seq : ind.sequenceInfo)
		{
			if (seq.LevelSequence != nullptr)
			{
				FMovieSceneSequencePlaybackSettings set;
				ALevelSequenceActor* dummyActor;

				ULevelSequencePlayer * t = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
					seq.LevelSequence, FMovieSceneSequencePlaybackSettings(), dummyActor);

				StageIndex[0].sequenceInfo[0].LevelSequencePlayer = t;

				/*seq.LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
					GetWorld(),
					seq.LevelSequence,
					set,
					dummyActor
				);*/
				
				//if(seq.LevelSequencePlayer)
				//	UE_LOG(LogTemp, Warning, TEXT("eeeeeeeee"));
				b++;
			}
		}
		a++;
	}

	if (StageIndex[0].sequenceInfo[0].LevelSequencePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ss"));
	}
}

// Called every frame
void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FLatentActionInfo latenInfo;
	UKismetSystemLibrary::Delay(this, 2, latenInfo);

	for (auto ind : StageIndex)
	{
		if (ind.isUse)
		{
			if (ind.sequenceInfo[ind.Wave].LevelSequencePlayer == nullptr)
			{
				int a = ind.Wave;
				UE_LOG(LogTemp, Warning, TEXT("%d"), a);
			}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Really"));
			//}
			//ind.sequenceInfo[ind.Wave].isPlay = true; //이거 끝까지 안쓰면 지우기
			//ind.sequenceInfo[ind.Wave].LevelSequencePlayer->Play();
			//float currentTime = UTimeManagementBlueprintLibrary::Conv_QualifiedFrameTimeToSeconds(ind.sequenceInfo[ind.Wave].LevelSequencePlayer->GetCurrentTime());
			//float endTime = UTimeManagementBlueprintLibrary::Conv_QualifiedFrameTimeToSeconds(ind.sequenceInfo[ind.Wave].LevelSequencePlayer->GetEndTime());
			//if (currentTime == endTime)
			//{
			//	if (ind.Wave == ind.sequenceInfo.Max())
			//	{
			//		ind.isUse = false;
			//	}
			//	ind.Wave++;
			//}
		}
	}
}

void AStageManager::SetterUse(int num)
{
	StageIndex[num].isUse = true;
}
