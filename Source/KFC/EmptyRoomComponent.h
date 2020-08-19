#pragma once

#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "EmptyRoomComponent.generated.h"

class AObstracles;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogEmptyRoomComponent, Log, All)

UCLASS(Blueprintable)
class UEmptyRoomComponent : public UPrimitiveComponent {
  GENERATED_UCLASS_BODY()

 public:
  virtual void DestroyComponent(bool bPromoteChildren = false) override;

  virtual void RecreateSubComponents();
  void SetDimensions(const FVector& Volume, const float SideWidth);

 protected:
  void DeleteAndCreateComponent(UStaticMeshComponent*& ComponentPtr, AObstracles* Owner);
  void RecreateComponent(UStaticMeshComponent*& ComponentPtr, AObstracles* Owner);

 protected:
  /** Mesh for all inner objects */
  UPROPERTY()
  UStaticMesh* MeshPtr;

  static const float kCubeSize;
  static const wchar_t* CollisionProfileName;

 private:
  UPROPERTY()
  UStaticMeshComponent* Roof;

  UPROPERTY()
  UStaticMeshComponent* Floor;

  UPROPERTY()
  UStaticMeshComponent* BackWall;
};
