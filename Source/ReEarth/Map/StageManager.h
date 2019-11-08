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

	//class AEnemyBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStageStruct> StageIndex;

	TArray<AEnemyBase*> EnemyArray;

	//시퀀스 시작 변경
	UFUNCTION(BlueprintCallable)
	void SetterUse(int num);

	//시퀀스동안 나온 적정보 수집
	UFUNCTION(BlueprintCallable)
	void AddEnemy(AEnemyBase* enemy);

};