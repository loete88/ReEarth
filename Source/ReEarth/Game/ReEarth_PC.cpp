// Fill out your copyright notice in the Description page of Project Settings.


#include "ReEarth_PC.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/VRHand/PlayerPawn.h"

void AReEarth_PC::ReqListen()
{
	UGameplayStatics::OpenLevel(this, TEXT("MultiMakeMap"), true, TEXT("listen"));
}

void AReEarth_PC::ReqConnect()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, df_SERVERIP);
}

void AReEarth_PC::TEST()
{

}
