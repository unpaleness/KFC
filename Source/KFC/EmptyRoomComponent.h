#pragma once

#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "EmptyRoomComponent.generated.h"

class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogEmptyRoomComponent, Log, All)

UCLASS(Blueprintable)
class UEmptyRoomComponent : public UPrimitiveComponent {
  GENERATED_UCLASS_BODY()

 public:
  void ReCreateSubComponents();
  void SetDimensions(const FVector& Volume, const float SideWidth);

 protected:
  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  UStaticMeshComponent* Roof;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  UStaticMeshComponent* Floor;

  /** Mesh for all inner objects */
  UPROPERTY()
  UStaticMesh* MeshPtr;
};