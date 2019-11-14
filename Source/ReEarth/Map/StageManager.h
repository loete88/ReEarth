// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageStruct.h"
#include "StageManager.generated.h"

UCLASS()
class REEARTH_API AStageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//스테이지 배열
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStageStruct> StageIndex;
	
	//시퀀스 동안 나온 적 정보 수집
	UFUNCTION(BlueprintCallable)
	void AddEnemy(AEnemyBase* enemy);
	
	//스테이지 관리
	//-----------------------------
	UFUNCTION(BlueprintCallable)
	void StartStage(int num);
	void PlayStage();
	void EndStage();
	int Wave = 0;
	int CurrentStage;
	//-----------------------------

private:
	//수집된 적
	UPROPERTY()
	TArray<AEnemyBase*> EnemyArray;

	//시퀀스 끝나는 시간
	FTimerHandle SequenceEndTimerHandle;

	//웨이브 도중 살아있는 적 확인
	FTimerHandle CheckAliveEnemyHandle;

	AActor* SpawnedBarrier;
};