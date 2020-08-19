#pragma once

#include "CoreMinimal.h"
#include "EmptyRoomComponent.h"
#include "RoomComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRoomComponent, Log, All)

class AObstracles;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS(Blueprintable)
class URoomComponent : public UEmptyRoomComponent {
  GENERATED_UCLASS_BODY()

 public:
  virtual void DestroyComponent(bool bPromoteChildren = false) override;

  virtual void RecreateSubComponents() override;
  void SetWallWithHole(const FVector& Volume,
                       const float SideWidth,
                       const float HoleSize,
                       const float MinWallPieceHeight);
  void RenewWallWithHole();

 protected:
  void DeleteAndCreateComponent(UBoxComponent*& ComponentPtr, AObstracles* Owner);
  void RecreateComponent(UBoxComponent*& ComponentPtr, AObstracles* Owner);

 private:
  UPROPERTY()
  UStaticMeshComponent* UpperWall;

  UPROPERTY()
  UStaticMeshComponent* LowerWall;

  UPROPERTY()
  UBoxComponent* Hole;

  FVector LastVolume_{};
  float LastSideWidth_{0.f};
  float LastHoleSize_{0.f};
  float LastMinWallPieceHeight_{0.f};
};
