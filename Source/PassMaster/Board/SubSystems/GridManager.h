#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridManager.generated.h"

USTRUCT()
struct FCell {
	
	GENERATED_BODY()

public:
	FCell() : _WorldPosition(FVector::ZeroVector), _CellPosition(FVector2D::ZeroVector), _CellSize(FVector2D::ZeroVector) {}

	FCell(FVector WorldPosition, FVector2D CellPosition,FVector2D CellSize) {
		_WorldPosition = WorldPosition;
		_CellPosition = CellPosition;
		_CellSize = CellSize;
	}

	FVector GetWorldPosition() {
		return _WorldPosition;
	}

	FVector2D GetGridPosition() {
		return _CellPosition;
	}

	FVector2D GetCellSize() {
		return _CellSize;
	}

	bool IsEmpty() {
		return _WorldPosition == FVector::ZeroVector && _CellPosition == FVector2D::ZeroVector && _CellSize == FVector2D::ZeroVector;
	}

private:
	FVector _WorldPosition;
	FVector2D _CellPosition;
	FVector2D _CellSize;
};

UCLASS()
class PASSMASTER_API UGridManager : public UWorldSubsystem {
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void SetupGrid(UBoardSubSystem* SubSystem, UWorld* World);

	UFUNCTION()
	void SetupVariables(AGridSpawner* GridSpawner,int& DistanceX,int& DistanceY,int& NumBlocksX,int& NumBlocksY,FVector& Start);
	
	UFUNCTION()
	void Display(FVector StartLocation, int CellCount);

	UFUNCTION()
	void DisplayFromGrid(int CellCount);

	void Deinitialize() override;

private:
	UFUNCTION()
	FCell GetCell(FVector2D CellPosition);

	UFUNCTION()
	FCell GetCellByWorld(FVector WorldPosition);

	UFUNCTION()
	UTexture2D* CreateTexture(float Width, float Height);

	void ResetTexture(uint8* TextureData);

	UPROPERTY(VisibleAnywhere)
	TArray<FCell> _GridCells;

	UPROPERTY()
	UTexture2D* _GridTexture;

	UPROPERTY()
	int32 _GridWidth;

	UPROPERTY()
	int32 _GridHeight;

	UPROPERTY()
	int32 _TextureWidth;
	
	UPROPERTY()
	int32 _TextureHeight;

	UPROPERTY()
	TObjectPtr<class UBoardSubSystem> _BoardSubSystem;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> _GridMaterial;

	UPROPERTY()
	TSoftObjectPtr<class AActor> _Grid;
};
