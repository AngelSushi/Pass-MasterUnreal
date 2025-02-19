#include "GridManager.h"
#include "Kismet/GameplayStatics.h"

#include "PassMaster/GameManager.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/Datas/IsleBoardDataAsset.h"
#include "PassMaster/Board/Actors/GridSpawner.h"

#include "Materials/MaterialInstanceDynamic.h"

void UGridManager::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

void UGridManager::SetupGrid(UBoardSubSystem* BoardSystem,UWorld* World) {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Grid Setup"));
	_BoardSubSystem = BoardSystem;

	AActor* GridSpawner = UGameplayStatics::GetActorOfClass(World, AGridSpawner::StaticClass());

	if (!GridSpawner) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Error When Initializing Grid"));
		return;
	}

	_Grid = BoardSystem->GetBoardAsset()->Grid;

	int DistanceX,DistanceY;
	FVector Start;
	SetupVariables(Cast<AGridSpawner>(GridSpawner), DistanceX, DistanceY, _GridWidth, _GridHeight, Start);

	for (int i = 0; i < _GridHeight; i++) {
		for (int j = 0; j < _GridWidth; j++) {
			FVector WorldPosition = FVector(Start.X - j * 15 * 100, Start.Y + i * 15 * 100,Start.Z);
			FVector2D CellPosition = FVector2D(j,i);
			FVector2D CellSize = FVector2D(15, 15);

			/*AActor* CellToSpawn = ((AGridSpawner*)GridSpawners[0])->_TestMesh;

			AActor* SpawnActor = GetWorld()->SpawnActor(CellToSpawn->GetClass(), &WorldPosition, &FRotator::ZeroRotator);
			SpawnActor->SetActorScale3D(FVector(15,15,15));
			*/

			_GridCells.Add(FCell(WorldPosition,CellPosition,CellSize));
		}
	}

	// Le plane doit avoir la taille de CellCount * 15
	_GridTexture = CreateTexture(20 * 15 * 4,20 * 15 * 4);

	_GridMaterial = UMaterialInstanceDynamic::Create(BoardSystem->GetBoardAsset()->PathMaterial, this);
	_Grid->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, _GridMaterial);

	DisplayFromGrid(9);
}

void UGridManager::SetupVariables(AGridSpawner* GridSpawner, int& DistanceX, int& DistanceY, int& GridWidth, int& GridHeight, FVector& Start) {
	DistanceX = FMath::Abs(GridSpawner->First->GetComponentLocation().X - GridSpawner->Second->GetComponentLocation().X);
	DistanceY = FMath::Abs(GridSpawner->First->GetComponentLocation().Y - GridSpawner->Second->GetComponentLocation().Y);

	GridWidth = DistanceX / 1500;
	GridHeight = DistanceY / 1500;

	Start = GridSpawner->First->GetComponentLocation() + FVector(-1500, 1500, 0) / 2;
}

void UGridManager::Display(FVector StartLocation, int CellCount) {}

void UGridManager::DisplayFromGrid(int CellCount) {
	if (!_GridTexture) {
		return;
	}
	
	FTexture2DMipMap& Mip = _GridTexture->PlatformData->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

	ResetTexture(static_cast<uint8*>(TextureData));

	int StartIndex = -CellCount;
	int EndIndex = CellCount + 1;

	for (int i = -1; i < 2; i += 2) {
		int CurrentStart = StartIndex;
		int CurrentEnd = EndIndex;

		for (int j = 0; j < EndIndex; j++) {
			for (int k = CurrentStart; k < CurrentEnd; k++) {
				int32 StartIndexX = _TextureWidth / 2 + ((15 * 4) * (k));
				int32 StartIndexY = _TextureHeight / 2 + ((15 * 4) * (j * i));

				for (int dj = 0; dj < 15 * 4; dj++) {
					for (int dk = 0; dk < 15 * 4; dk++) {						
						int32 PixelX = StartIndexX + static_cast<int32>(dk) % _TextureWidth;
						int32 PixelY = StartIndexY + static_cast<int32>(dj) % _TextureHeight;

						if (PixelX >= 0 && PixelX < _TextureWidth && PixelY >= 0 && PixelY < _TextureHeight) {
							int32 PixelIndex = PixelY * _TextureWidth + PixelX;
							uint8* DstPtr = static_cast<uint8*>(TextureData) + PixelIndex * 4;

							if (j == 0 && k == CurrentEnd - 1 && dk >= 50) {
								//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Green"));
								DstPtr[0] = 0;
								DstPtr[1] = 255;
								DstPtr[2] = 0;
								DstPtr[3] = 255;
							}
							else {
								DstPtr[0] = 0;
								DstPtr[1] = 0;
								DstPtr[2] = 0;
								DstPtr[3] = 255;
							}
						}
					}
				}
			}

			CurrentStart++;
			CurrentEnd--;
		}
	}
	
	Mip.BulkData.Unlock();
	_GridTexture->UpdateResource();

	_GridMaterial->SetTextureParameterValue("Texture", _GridTexture); 
	//_Grid->SetActorLocation();

}

FCell UGridManager::GetCell(FVector2D CellPosition) {
	for (FCell Cell : _GridCells) {
		if (Cell.GetGridPosition() == CellPosition) {
			return Cell;
		}
	}

	return FCell();
}

FCell UGridManager::GetCellByWorld(FVector WorldPosition) {
	for (FCell Cell : _GridCells) {
		if (WorldPosition.X >= Cell.GetWorldPosition().X - Cell.GetCellSize().X / 2 && WorldPosition.X <= Cell.GetWorldPosition().X + Cell.GetCellSize().X / 2 
			&& WorldPosition.Z >= Cell.GetWorldPosition().Z - Cell.GetCellSize().Y / 2 && WorldPosition.Z <= Cell.GetWorldPosition().Z + Cell.GetCellSize().Y / 2) {
			return Cell;
		}
	}

	return FCell();
}

UTexture2D* UGridManager::CreateTexture(float Width, float Height) {
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height,PF_B8G8R8A8);
	Texture->UpdateResource();

	Texture->MipGenSettings = TMGS_NoMipmaps;
	Texture->Filter = TF_Nearest;
	Texture->SRGB = false;

	_TextureWidth = Width;
	_TextureHeight = Height;

	return Texture;
}

void UGridManager::ResetTexture(uint8* TextureData) {
	for (int32 Y = 0; Y < _TextureHeight; Y++) {
		for (int32 X = 0; X < _TextureWidth; X++) {
			int32 PixelIndex = (Y * _TextureWidth + X) * 4; 
			uint8* DestPtr = static_cast<uint8*>(TextureData) + PixelIndex;
			DestPtr[0] = 255;
			DestPtr[1] = 255;
			DestPtr[2] = 255;
			DestPtr[3] = 255;
		}
	}
}

void UGridManager::Deinitialize() {
	Super::Deinitialize();
}