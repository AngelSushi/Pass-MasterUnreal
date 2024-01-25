// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PassMasterGameMode.generated.h"

class UIsleBoardDataAsset;

UCLASS(minimalapi)
class APassMasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APassMasterGameMode();

	UPROPERTY(EditAnywhere)
	UIsleBoardDataAsset* BoardAsset;
};



