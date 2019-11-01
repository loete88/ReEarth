// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReEarth_PC.generated.h"


#define df_SERVERIP TEXT("open 192.168.0.152")

/**
 * 
 */
UCLASS()
class REEARTH_API AReEarth_PC : public APlayerController
{
	GENERATED_BODY()
	
public:

	//ReqListen 함수
	//--------------------------------------------------------------
	//해당 Controller를 가진 Client를 Listen상태로 만든다.
	void ReqListen();
	//--------------------------------------------------------------

	//ReqConnect 함수
	//--------------------------------------------------------------
	//해당 Controller를 가진 Client를 Listen 
	void ReqConnect();
	//--------------------------------------------------------------

	//TEST함수
	void TEST();



};
