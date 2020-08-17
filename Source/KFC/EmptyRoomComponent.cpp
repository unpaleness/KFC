#include "EmptyRoomComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Obstracles.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogEmptyRoomComponent)

namespace {

const float kCubeSize{100.f};
const auto CollisionProfileName = TEXT("OverlapAllDynamic");

}  // namespace

UEmptyRoomComponent::UEmptyRoomComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  static const auto MeshCube =
      ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
  MeshPtr = MeshCube.Object;
}

void UEmptyRoomComponent::ReCreateSubComponents() {
  const auto Owner = Cast<AObstracles>(GetOwner());
  if (!IsValid(Owner)) {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Owner is not of class AObstracles!"));
    return;
  }

  if (IsValid(Roof)) {
    Roof->DestroyComponent();
  }

  Roof = NewObject<UStaticMeshComponent>(Owner, MakeUniqueObjectName(GetWorld(), UStaticMeshComponent::StaticClass()));
  if (!IsValid(Roof)) {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Component creation failed!"));
    return;
  }

  Roof->SetStaticMesh(MeshPtr);
  Roof->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
  Roof->SetCollisionProfileName(CollisionProfileName);
  Roof->RegisterComponent();
  Roof->OnComponentBeginOverlap.AddDynamic(Owner, &AObstracles::ProcessSolidBeginOverlap);

  if (IsValid(Floor)) {
    Floor->DestroyComponent();
  }

  Floor = NewObject<UStaticMeshComponent>(Owner, MakeUniqueObjectName(GetWorld(), UStaticMeshComponent::StaticClass()));
  if (!IsValid(Floor)) {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Component creation failed!"));
    return;
  }

  Floor->SetStaticMesh(MeshPtr);
  Floor->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
  Floor->SetCollisionProfileName(CollisionProfileName);
  Floor->RegisterComponent();
  Floor->OnComponentBeginOverlap.AddDynamic(Owner, &AObstracles::ProcessSolidBeginOverlap);
}

void UEmptyRoomComponent::SetDimensions(const FVector& Volume, const float SideWidth) {
  if (IsValid(Roof)) {
    Roof->SetRelativeLocation({0.f, 0.f, (Volume.Z + SideWidth) / 2.f});
    Roof->SetRelativeScale3D(FVector(Volume.X, Volume.Y, SideWidth) / kCubeSize);
    UE_LOG(LogEmptyRoomComponent, Log, TEXT("Roof set"));
  } else {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Roof is invalid!"));
  }

  if (IsValid(Floor)) {
    Floor->SetRelativeLocation({0.f, 0.f, -(Volume.Z + SideWidth) / 2.f});
    Floor->SetRelativeScale3D(FVector(Volume.X, Volume.Y, SideWidth) / kCubeSize);
    UE_LOG(LogEmptyRoomComponent, Log, TEXT("Floor set"));
  } else {
    UE_LOG(LogEmptyRoomComponent, Warning, TEXT("Floor is invalid!"));
  }
}
