#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

#include "Obstracles.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogObstracles, Log, All)

UCLASS()
class AObstracles : public AActor {
  GENERATED_BODY()

 public:
  AObstracles();
  virtual void PostActorCreated() override;
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
  float GetRandomizedHoleHeight() const;
  void RandomizeHoleHeight();

  FVector GetLowerPieceLocation(float Offset) const;
  FVector GetLowerPieceScale3D() const;
  FVector GetUpperPieceLocation(float Offset) const;
  FVector GetUpperPieceScale3D() const;
  FVector GetHoleLocation(float Offset) const;
  FVector GetHoleScale3D() const;

  UStaticMeshComponent* CreateWall();
  UBoxComponent* CreateHole();

  void RecreateWalls();
  void RecreateHoles();

 public:
  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  USceneComponent* Root;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  UStaticMeshComponent* Roof;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  UStaticMeshComponent* Floor;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  TArray<UStaticMeshComponent*> Walls;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
  TArray<UBoxComponent*> Holes;

 protected:
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float Height{600.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float StepWidth{400.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float DistanceToFirstWall{500.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float MaxLeftDistanceToReplaceWall{1000.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float HoleSize{200.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float MinWallPieceHeight{50.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float Speed{200.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float Depth{200.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
  float HoleBoxScaleMultiplier{1.5f};

  UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
  int32 Level{0};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics", meta = (ClampMin = "2", ClampMax = "40"))
  int32 WallsCacheSize{10};

  UPROPERTY()
  UStaticMesh* MeshPtr;

 private:
  bool bIsRunning_{false};
  float NextHoleHeight_{0.f};
  bool bPieceTypeSemaphore{false};
  int32 WallsCacheSize_{10};
  float DifficultyMultiplier_{1.f};
  bool bIsInHole{false};
};
