#include "Obstracles.h"

#include "ChickenCharacter.h"
#include "ChickenPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RoomComponent.h"

#include "KFCGameMode.h"

DEFINE_LOG_CATEGORY(LogObstracles)

namespace {

const auto CollisionProfileName = TEXT("OverlapAllDynamic");

}  // namespace

AObstracles::AObstracles() {
  PrimaryActorTick.bCanEverTick = true;

  Root = CreateDefaultSubobject<USceneComponent>("Root");
  SetRootComponent(Root);

  static const auto MeshCube =
      ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
  MeshPtr = MeshCube.Object;

  Room = CreateDefaultSubobject<URoomComponent>("Room");
  Room->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
  Room->RegisterComponent();
}

void AObstracles::PostActorCreated() {
  // Room->OnComponentBeginOverlap.AddDynamic(this, &AObstracles::ProcessSolidBeginOverlap);
}

void AObstracles::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (bIsRunning_) {
    for (auto Wall : Walls) {
      Wall->AddRelativeLocation({0.f, DeltaTime * Speed * DifficultyMultiplier_, 0.f});
      if (Wall->GetRelativeLocation().Y >= MaxLeftDistanceToReplaceWall) {
        const auto Offset = Wall->GetRelativeLocation().Y - StepWidth * RoomsCacheSize_;
        const auto NewLocation = bPieceTypeSemaphore ? GetLowerPieceLocation(Offset) : GetUpperPieceLocation(Offset);
        const auto NewScale3D = bPieceTypeSemaphore ? GetLowerPieceScale3D() : GetUpperPieceScale3D();
        Wall->SetRelativeLocation(NewLocation);
        Wall->SetRelativeScale3D(NewScale3D);
        bPieceTypeSemaphore = !bPieceTypeSemaphore;
      }
    }
    for (auto Hole : Holes) {
      Hole->AddRelativeLocation({0.f, DeltaTime * Speed * DifficultyMultiplier_, 0.f});
      if (Hole->GetRelativeLocation().Y >= MaxLeftDistanceToReplaceWall) {
        const auto Offset = Hole->GetRelativeLocation().Y - StepWidth * RoomsCacheSize_;
        Hole->SetRelativeLocation(GetHoleLocation(Offset));
        Hole->SetRelativeScale3D(GetHoleScale3D());
      }
    }
  }
}

void AObstracles::OnConstruction(const FTransform& Transform) {
  Room->ReCreateSubComponents();
  Room->SetRelativeLocation({0.f, 0.f, 300.f});
  Room->SetDimensions({Depth, StepWidth, Height}, 20.f);

  Reset();
}

void AObstracles::OnStartMatch_Implementation() {
  UE_LOG(LogObstracles, Log, TEXT("Match starts"));

  Reset();

  bIsRunning_ = true;
}

void AObstracles::OnEndMatch_Implementation() {
  UE_LOG(LogObstracles, Log, TEXT("Match ends"));

  bIsRunning_ = false;
}

void AObstracles::ProcessHoleBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                         AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp,
                                                         int32 OtherBodyIndex,
                                                         bool bFromSweep,
                                                         const FHitResult& SweepResult) {
  if (this == OtherActor) {
    return;
  }

  UE_LOG(LogObstracles, Log, TEXT("Hole began overlap with %s"), *OtherActor->GetClass()->GetName());

  bIsInHole = true;
}

void AObstracles::ProcessHoleEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex) {
  if (this == OtherActor) {
    return;
  }

  UE_LOG(LogObstracles, Log, TEXT("Hole ended overlap with %s"), *OtherActor->GetClass()->GetName());

  if (!bIsInHole)
    return;

  bIsInHole = false;

  ++Level;
  DifficultyMultiplier_ = 1.f + Level / 10.f;
  RandomizeHoleHeight();
}

void AObstracles::ProcessSolidBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                          AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp,
                                                          int32 OtherBodyIndex,
                                                          bool bFromSweep,
                                                          const FHitResult& SweepResult) {
  if (this == OtherActor) {
    return;
  }

  UE_LOG(LogObstracles, Log, TEXT("Solid began overlap with %s"), *OtherActor->GetClass()->GetName());

  auto ChickenCharacter = Cast<AChickenCharacter>(OtherActor);

  if (!IsValid(ChickenCharacter))
    return;

  auto ChickenController = Cast<AChickenPlayerController>(ChickenCharacter->GetController());

  if (!IsValid(ChickenController))
    return;

  ChickenController->ProcessChickenHit();
}

void AObstracles::BeginPlay() {
  Super::BeginPlay();

  auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
  if (IsValid(GameMode)) {
    GameMode->StartMatchDelegate.AddDynamic(this, &AObstracles::OnStartMatch);
    GameMode->EndMatchDelegate.AddDynamic(this, &AObstracles::OnEndMatch);
  }
}

void AObstracles::Reset() {
  RoomsCacheSize_ = RoomsCacheSize;

  RecreateWalls();
  RecreateHoles();

  for (int32 i = 0; i < RoomsCacheSize_; ++i) {
    RandomizeHoleHeight();
    auto Wall1 = Walls[i * 2];
    if (IsValid(Wall1)) {
      Wall1->SetRelativeLocation(GetLowerPieceLocation(-DistanceToFirstWall - StepWidth * i));
      Wall1->SetRelativeScale3D(GetLowerPieceScale3D());
    } else {
      UE_LOG(LogObstracles, Warning, TEXT("Lower wall is invalid!"));
    }

    auto Wall2 = Walls[i * 2 + 1];
    if (IsValid(Wall2)) {
      Wall2->SetRelativeLocation(GetUpperPieceLocation(-DistanceToFirstWall - StepWidth * i));
      Wall2->SetRelativeScale3D(GetUpperPieceScale3D());
    } else {
      UE_LOG(LogObstracles, Warning, TEXT("Upper wall is invalid!"));
    }

    auto Hole = Holes[i];
    if (IsValid(Hole)) {
      Hole->SetRelativeLocation(GetHoleLocation(-DistanceToFirstWall - StepWidth * i));
      Hole->SetRelativeScale3D(GetHoleScale3D());
    } else {
      UE_LOG(LogObstracles, Warning, TEXT("Hole is invalid!"));
    }
  }

  Level = 0;
  DifficultyMultiplier_ = 1.f;
}

float AObstracles::GetRandomizedHoleHeight() const {
  const auto Range = Height - 2.f * MinWallPieceHeight - HoleSize;
  return FMath::RandRange(-Range / 2.f, Range / 2.f);
}

void AObstracles::RandomizeHoleHeight() {
  NextHoleHeight_ = GetRandomizedHoleHeight();
}

FVector AObstracles::GetLowerPieceLocation(float Offset) const {
  return {0.f, Offset, (NextHoleHeight_ - (Height + HoleSize) / 2.f) / 2.f};
}
FVector AObstracles::GetLowerPieceScale3D() const {
  return {Depth / 100.f, 0.2f, ((Height - HoleSize) / 2.f + NextHoleHeight_) / 100.f};
}
FVector AObstracles::GetUpperPieceLocation(float Offset) const {
  return {0.f, Offset, (NextHoleHeight_ + (Height + HoleSize) / 2.f) / 2.f};
}
FVector AObstracles::GetUpperPieceScale3D() const {
  return {Depth / 100.f, 0.2f, ((Height - HoleSize) / 2.f - NextHoleHeight_) / 100.f};
}
FVector AObstracles::GetHoleLocation(float Offset) const {
  return {0.f, Offset, NextHoleHeight_};
}
FVector AObstracles::GetHoleScale3D() const {
  return FVector(Depth / 100.f, 0.2f, HoleSize / 100.f) * HoleBoxScaleMultiplier;
}

UStaticMeshComponent* AObstracles::CreateWall() {
  auto Wall =
      NewObject<UStaticMeshComponent>(this, MakeUniqueObjectName(GetWorld(), UStaticMeshComponent::StaticClass()));
  if (!IsValid(Wall)) {
    UE_LOG(LogObstracles, Warning, TEXT("Create new wall piece failed!"));
    return nullptr;
  }

  Wall->SetStaticMesh(MeshPtr);
  Wall->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
  Wall->SetCollisionProfileName(CollisionProfileName);
  Wall->RegisterComponent();
  Wall->OnComponentBeginOverlap.AddDynamic(this, &AObstracles::ProcessSolidBeginOverlap);

  return Wall;
}

UBoxComponent* AObstracles::CreateHole() {
  auto Hole = NewObject<UBoxComponent>(this, MakeUniqueObjectName(GetWorld(), UBoxComponent::StaticClass()));
  if (!IsValid(Hole)) {
    UE_LOG(LogObstracles, Warning, TEXT("Create new wall hole failed!"));
    return nullptr;
  }

  Hole->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
  Hole->SetCollisionProfileName(CollisionProfileName);
  Hole->RegisterComponent();
  Hole->OnComponentBeginOverlap.AddDynamic(this, &AObstracles::ProcessHoleBeginOverlap);
  Hole->OnComponentEndOverlap.AddDynamic(this, &AObstracles::ProcessHoleEndOverlap);

  return Hole;
}

void AObstracles::RecreateWalls() {
  for (auto Wall : Walls) {
    if (IsValid(Wall)) {
      Wall->DestroyComponent();
    }
  }
  Walls.Empty();
  Walls.Reserve(RoomsCacheSize_ * 2);
  for (int32 i = 0; i < RoomsCacheSize_ * 2; ++i) {
    auto Wall = CreateWall();
    if (IsValid(Wall)) {
      Walls.Add(Wall);
    }
  }
}

void AObstracles::RecreateHoles() {
  for (auto Hole : Holes) {
    if (IsValid(Hole)) {
      Hole->DestroyComponent();
    }
  }
  Holes.Empty();
  Holes.Reserve(RoomsCacheSize_);
  for (int32 i = 0; i < RoomsCacheSize_; ++i) {
    auto Hole = CreateHole();
    if (IsValid(Hole)) {
      Holes.Add(Hole);
    }
  }
}
