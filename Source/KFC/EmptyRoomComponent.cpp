#include "EmptyRoomComponent.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Obstracles.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogEmptyRoomComponent)

const float UEmptyRoomComponent::kCubeSize{100.f};
const wchar_t* UEmptyRoomComponent::CollisionProfileName{TEXT("OverlapAllDynamic")};

UEmptyRoomComponent::UEmptyRoomComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  static const auto MeshCube =
      ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
  MeshPtr = MeshCube.Object;
}

void UEmptyRoomComponent::DestroyComponent(bool bPromoteChildren /*= false*/) {
  if (IsValid(Roof)) {
    Roof->DestroyComponent();
  }
  if (IsValid(Floor)) {
    Floor->DestroyComponent();
  }
  if (IsValid(BackWall)) {
    BackWall->DestroyComponent();
  }

  Super::DestroyComponent(bPromoteChildren);
}

void UEmptyRoomComponent::RecreateSubComponents() {
  const auto Owner = Cast<AObstracles>(GetOwner());
  if (!IsValid(Owner)) {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Owner is not of class AObstracles!"));
    return;
  }

  RecreateComponent(Roof, Owner);
  RecreateComponent(Floor, Owner);
  RecreateComponent(BackWall, Owner);
}

void UEmptyRoomComponent::SetDimensions(const FVector& Volume, const float SideWidth) {
  if (IsValid(Roof)) {
    Roof->SetRelativeLocation({0.f, 0.f, (Volume.Z + SideWidth) / 2.f});
    Roof->SetRelativeScale3D(FVector(Volume.X, Volume.Y, SideWidth) / kCubeSize);
  } else {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Roof is invalid!"));
  }

  if (IsValid(Floor)) {
    Floor->SetRelativeLocation({0.f, 0.f, -(Volume.Z + SideWidth) / 2.f});
    Floor->SetRelativeScale3D(FVector(Volume.X, Volume.Y, SideWidth) / kCubeSize);
  } else {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Floor is invalid!"));
  }

  if (IsValid(BackWall)) {
    BackWall->SetRelativeLocation({-(Volume.X + SideWidth) / 2.f, 0.f, 0.f});
    BackWall->SetRelativeScale3D(FVector(SideWidth, Volume.Y, Volume.Z) / kCubeSize);
  } else {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("BackWall is invalid!"));
  }
}

void UEmptyRoomComponent::DeleteAndCreateComponent(UStaticMeshComponent*& ComponentPtr, AObstracles* Owner) {
  if (IsValid(ComponentPtr)) {
    ComponentPtr->DestroyComponent();
  }

  ComponentPtr =
      NewObject<UStaticMeshComponent>(Owner, MakeUniqueObjectName(GetWorld(), UStaticMeshComponent::StaticClass()));
}

void UEmptyRoomComponent::RecreateComponent(UStaticMeshComponent*& ComponentPtr, AObstracles* Owner) {
  DeleteAndCreateComponent(ComponentPtr, Owner);

  ComponentPtr->SetStaticMesh(MeshPtr);
  ComponentPtr->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
  ComponentPtr->SetCollisionProfileName(CollisionProfileName);
  ComponentPtr->RegisterComponent();
  ComponentPtr->OnComponentBeginOverlap.AddDynamic(Owner, &AObstracles::ProcessSolidBeginOverlap);
}
