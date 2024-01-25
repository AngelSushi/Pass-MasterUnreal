// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

class APassMasterCharacter;

USTRUCT(BlueprintType)
struct PASSMASTER_API FPlayerData
{

	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	APassMasterCharacter* Actor;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int16 PlayerOrder;

	UPROPERTY(EditAnywhere)
	int16 Rank;

	UPROPERTY(EditAnywhere)
	int16 Coins;

	UPROPERTY(EditAnywhere)
	int16 Cards;

public:
	FPlayerData() : Actor(nullptr), Name(""), PlayerOrder(0), Rank(0), Coins(0), Cards(0) {}

	FPlayerData(APassMasterCharacter* InActor, const FString& InName,int16 InOrder, int16 InRank, int16 InCoins, int16 InCards): 
		Actor(InActor), Name(InName), PlayerOrder(InOrder), Rank(InRank), Coins(InCoins), Cards(InCards) {}


	FORCEINLINE class APassMasterCharacter* GetActor() const { return Actor; }
	FORCEINLINE class FString GetName() const { return Name; }

	/* Get the value of DiceResult to define Order Of Player */
	FORCEINLINE int16 GetOrder() const { return PlayerOrder;  }
	FORCEINLINE int16 GetRank() const { return Rank; }
	FORCEINLINE int16 GetCoins() const { return Coins; }
	FORCEINLINE int16 GetCards() const { return Cards; }

	FORCEINLINE void SetOrder(int16 Order) { this->PlayerOrder = Order; }

	bool operator==(const FPlayerData& otherData) const {
		return GetActor() == otherData.GetActor();
	}

	// Inventory ; Coins : Cards : Rank
};
