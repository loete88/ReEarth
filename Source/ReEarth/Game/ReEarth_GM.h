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

	//GameStart�Լ�
	//----------------------
	//MainMap���� �̵�
	void GameStart();
	//----------------------

	//GameExit�Լ�
	//----------------------
	//���� ����
	void GameExit();
	//----------------------

	//GoToLobby�Լ�
	//----------------------
	//StartMap���� �̵�
	void GoToLobby();
	//----------------------

	UPROPERTY(EditAnywhere)
	UTexture * LoadTexture;
};
