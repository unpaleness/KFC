#include "RoomComponent.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Obstracles.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogRoomComponent)

namespace {

float GetRandomizedHoleHeight(const float Height, const float HoleSize, const float MinWallPieceHeight) {
  const auto Range = Height - 2.f * MinWallPieceHeight - HoleSize;
  return FMath::RandRange(-Range / 2.f, Range / 2.f);
}

}  // namespace

URoomComponent::URoomComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void URoomComponent::DestroyComponent(bool bPromoteChildren /*= false*/) {
  if (IsValid(UpperWall)) {
    UpperWall->DestroyComponent();
  }
  if (IsValid(LowerWall)) {
    LowerWall->DestroyComponent();
  }
  if (IsValid(Hole)) {
    Hole->DestroyComponent();
  }

  Super::DestroyComponent(bPromoteChildren);
}

void URoomComponent::RecreateSubComponents() {
  Super::RecreateSubComponents();

  const auto Owner = Cast<AObstracles>(GetOwner());
  if (!IsValid(Owner)) {
    UE_LOG(LogRoomComponent, Warning, TEXT("Owner is not of class AObstracles!"));
    return;
  }

  Super::RecreateComponent(UpperWall, Owner);
  Super::RecreateComponent(LowerWall, Owner);
  RecreateComponent(Hole, Owner);
}

void URoomComponent::SetWallWithHole(const FVector& Volume,
                                     const float SideWidth,
                                     const float HoleSize,
                                     const float MinWallPieceHeight) {
  LastVolume_ = Volume;
  LastSideWidth_ = SideWidth;
  LastHoleSize_ = HoleSize;
  LastMinWallPieceHeight_ = MinWallPieceHeight;

  RenewWallWithHole();
}

void URoomComponent::RenewWallWithHole() {
  const auto HoleHeight = GetRandomizedHoleHeight(LastVolume_.Z, LastHoleSize_, LastMinWallPieceHeight_);

  if (IsValid(UpperWall)) {
    UpperWall->SetRelativeLocation({0.f, 0.f, (HoleHeight + (LastVolume_.Z + LastHoleSize_) / 2.f) / 2.f});
    UpperWall->SetRelativeScale3D(
        FVector(LastVolume_.X, LastSideWidth_, ((LastVolume_.Z - LastHoleSize_) / 2.f - HoleHeight)) / kCubeSize);
  } else {
    UE_LOG(LogRoomComponent, Warning, TEXT("UpperWall is invalid!"));
  }

  if (IsValid(LowerWall)) {
    LowerWall->SetRelativeLocation({0.f, 0.f, (HoleHeight - (LastVolume_.Z + LastHoleSize_) / 2.f) / 2.f});
    LowerWall->SetRelativeScale3D(
        FVector(LastVolume_.X, LastSideWidth_, ((LastVolume_.Z - LastHoleSize_) / 2.f + HoleHeight)) / kCubeSize);
  } else {
    UE_LOG(LogRoomComponent, Warning, TEXT("LowerWall is invalid!"));
  }

  if (IsValid(Hole)) {
    Hole->SetRelativeLocation({0.f, 0.f, HoleHeight});
    Hole->SetBoxExtent(FVector(LastVolume_.X, LastSideWidth_, LastHoleSize_) / 2.f);
  } else {
    UE_LOG(LogRoomComponent, Warning, TEXT("Hole is invalid!"));
  }
}

void URoomComponent::DeleteAndCreateComponent(UBoxComponent*& ComponentPtr, AObstracles* Owner) {
  if (IsValid(ComponentPtr)) {
    ComponentPtr->DestroyComponent();
  }

  ComponentPtr = NewObject<UBoxComponent>(Owner, MakeUniqueObjectName(GetWorld(), UBoxComponent::StaticClass()));
}

void URoomComponent::RecreateComponent(UBoxComponent*& ComponentPtr, AObstracles* Owner) {
  DeleteAndCreateComponent(ComponentPtr, Owner);

  ComponentPtr->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
  ComponentPtr->SetCollisionProfileName(CollisionProfileName);
  ComponentPtr->RegisterComponent();
  ComponentPtr->OnComponentBeginOverlap.AddDynamic(Owner, &AObstracles::ProcessHoleBeginOverlap);
  ComponentPtr->OnComponentEndOverlap.AddDynamic(Owner, &AObstracles::ProcessHoleEndOverlap);
}