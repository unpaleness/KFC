#include "Obstracles.h"

#include "ChickenCharacter.h"
#include "ChickenPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EmptyRoomComponent.h"
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
}

void AObstracles::PostActorCreated() {}

void AObstracles::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (bIsRunning_) {
    TickEntryRooms(DeltaTime);
    TickRooms(DeltaTime);
  }
}

void AObstracles::OnConstruction(const FTransform& Transform) {
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
  RecreateEntryRooms();
  RecreateRooms();

  Level = 0;
  DifficultyMultiplier_ = 1.f;
}

void AObstracles::RecreateEntryRooms() {
  for (auto EntryRoom : EntryRooms) {
    if (IsValid(EntryRoom)) {
      EntryRoom->DestroyComponent();
    }
  }
  EntryRooms.Empty();
  EntryRooms.Reserve(EntryRoomsCacheSize);
  for (int32 i = 0; i < EntryRoomsCacheSize; ++i) {
    auto EntryRoom =
        NewObject<UEmptyRoomComponent>(this, MakeUniqueObjectName(GetWorld(), UEmptyRoomComponent::StaticClass()));
    EntryRoom->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
    EntryRoom->RegisterComponent();
    EntryRoom->RecreateSubComponents();
    EntryRoom->SetRelativeLocation({0.f, -DistanceToFirstRoom + StepWidth * (i + 0.5f), 0.f});
    EntryRoom->SetDimensions({Depth, StepWidth, Height}, WallWidth);
    if (IsValid(EntryRoom)) {
      EntryRooms.Add(MoveTemp(EntryRoom));
    }
  }
}

void AObstracles::RecreateRooms() {
  for (auto Room : Rooms) {
    if (IsValid(Room)) {
      Room->DestroyComponent();
    }
  }
  Rooms.Empty();
  Rooms.Reserve(RoomsCacheSize);
  for (int32 i = 0; i < RoomsCacheSize; ++i) {
    auto Room = NewObject<URoomComponent>(this, MakeUniqueObjectName(GetWorld(), URoomComponent::StaticClass()));
    Room->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
    Room->RegisterComponent();
    Room->RecreateSubComponents();
    Room->SetRelativeLocation({0.f, -DistanceToFirstRoom - StepWidth * (i + 0.5f), 0.f});
    Room->SetDimensions({Depth, StepWidth, Height}, WallWidth);
    Room->SetWallWithHole({Depth, StepWidth, Height}, WallWidth, HoleSize, MinWallPieceHeight);
    if (IsValid(Room)) {
      Rooms.Add(MoveTemp(Room));
    }
  }
}

void AObstracles::TickEntryRooms(const float DeltaTime) {
  for (auto EntryRoom : EntryRooms) {
    if (EntryRoom->GetRelativeLocation().Y <= MaxLeftDistanceToReplaceRoom + StepWidth / 2.f) {
      EntryRoom->AddRelativeLocation({0.f, DeltaTime * Speed * DifficultyMultiplier_, 0.f});
    }
  }
}

void AObstracles::TickRooms(const float DeltaTime) {
  for (auto Room : Rooms) {
    Room->AddRelativeLocation({0.f, DeltaTime * Speed * DifficultyMultiplier_, 0.f});
    if (Room->GetRelativeLocation().Y > MaxLeftDistanceToReplaceRoom - StepWidth / 2.f) {
      Room->AddRelativeLocation({0.f, -StepWidth * RoomsCacheSize, 0.f});
      Room->RenewWallWithHole();
    }
  }
}
