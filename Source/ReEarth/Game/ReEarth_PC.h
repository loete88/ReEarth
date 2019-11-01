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

	//ReqListen �Լ�
	//--------------------------------------------------------------
	//�ش� Controller�� ���� Client�� Listen���·� �����.
	void ReqListen();
	//--------------------------------------------------------------

	//ReqConnect �Լ�
	//--------------------------------------------------------------
	//�ش� Controller�� ���� Client�� Listen 
	void ReqConnect();
	//--------------------------------------------------------------

	//TEST�Լ�
	void TEST();



};
