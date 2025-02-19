// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

struct FPlayerData;

UCLASS()
class PASSMASTER_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 

	//UPROPERTY(BlueprintReadWrite)
//	TArray<FPlayerData> PlayerUIDatas;

	//UFUNCTION(BlueprintCallable)
	//TArray<FPlayerData> GetPlayerDatas() { return PlayerUIDatas; }

//	UFUNCTION(BlueprintCallable)	
//	void SetPlayerUIDatas(TArray<FPlayerData> NewPlayerUIDatas) { PlayerUIDatas = NewPlayerUIDatas; }
};
