#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

#include "Obstracles.generated.h"

class UEmptyRoomComponent;
class URoomComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogObstracles, Log, All)

UCLASS()
class AObstracles : public AActor {
  GENERATED_BODY()

 public:
  AObstracles();
  virtual void Tick(float DeltaTime) override;
  virtual void OnConstruction(const FTransform& Transform) override;

  UFUNCTION(BlueprintNativeEvent)
  void OnStartMatch();

  UFUNCTION(BlueprintNativeEvent)
  void OnEndMatch();

  UFUNCTION(BlueprintNativeEvent)
  void ProcessHoleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult);

  UFUNCTION(BlueprintNativeEvent)
  void ProcessHoleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                             AActor* OtherActor,
                             UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex);

  UFUNCTION(BlueprintNativeEvent)
  void ProcessSolidBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex,
                                bool bFromSweep,
                                const FHitResult& SweepResult);

 protected:
  virtual void BeginPlay() override;

 private:
  void Reset();
  void RecreateEntryRooms();
  void RecreateRooms();

  void TickRooms(const float DeltaTime);

 protected:
  UPROPERTY()
  TArray<UEmptyRoomComponent*> EntryRooms;

  UPROPERTY()
  TArray<URoomComponent*> Rooms;

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float Height{600.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float StepWidth{400.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float DistanceToFirstRoom{800.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float MaxLeftDistanceToReplaceRoom{1000.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float HoleSize{200.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float MinWallPieceHeight{50.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float Speed{200.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float Depth{200.f};

  UPROPERTY(EditAnywhere, Category = "Mechanics")
  float WallWidth{20.f};

  /** Amount of main rooms with obstracles */
  UPROPERTY(EditAnywhere, Category = "Mechanics", meta = (ClampMin = "1", ClampMax = "40"))
  int32 RoomsCacheSize{5};

  /** Amount of empty rooms to warm up */
  UPROPERTY(EditAnywhere, Category = "Mechanics", meta = (ClampMin = "1", ClampMax = "40"))
  int32 EntryRoomsCacheSize{5};

  UPROPERTY(BlueprintReadOnly, Category = "Mechanics")
  int32 Level{0};

  /** Mesh for all inner objects */
  UPROPERTY()
  UStaticMesh* MeshPtr;

 private:
  bool bIsRunning_{false};
  float DifficultyMultiplier_{1.f};
  bool bIsInHole{false};
  FVector SpawnPoint_{0.f, 0.f, 0.f};
};
