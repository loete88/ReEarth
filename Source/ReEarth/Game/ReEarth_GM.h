// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReEarth_GM.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API AReEarth_GM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//GameStart함수
	//----------------------
	//MainMap으로 이동
	void GameStart();
	//----------------------

	//GameExit함수
	//----------------------
	//게임 종료
	void GameExit();
	//----------------------

	//GoToLobby함수
	//----------------------
	//StartMap으로 이동
	void GoToLobby();
	//----------------------

	UPROPERTY(EditAnywhere)
	UTexture * LoadTexture;
};
