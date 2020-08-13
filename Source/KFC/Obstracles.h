#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstracles.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogObstracles, Log, All)

UCLASS()
class AObstracles : public AActor {
	GENERATED_BODY()

public:
	AObstracles();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnStartMatch();

	UFUNCTION()
	void OnEndMatch();

	UFUNCTION()
	void ProcessHoleBeginOverlap(AActor* SelfActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

private:
	void Reset();
	float GetRandomizedHoleHeight() const;
	void RandomizeHoleHieght();

	FVector GetLowerPieceLocation(float Offset) const;
	FVector GetLowerPieceScale3D() const;
	FVector GetUpperPieceLocation(float Offset) const;
	FVector GetUpperPieceScale3D() const;
	FVector GetHoleLocation(float Offset) const;
	FVector GetHoleScale3D() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Roof;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Floor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TArray<UStaticMeshComponent*> WallPieces;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TArray<UBoxComponent*> WallHoles;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float Height{ 600.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float StepWidth{ 400.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float DistanceToFirstWall{ 500.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float MaxLeftDistanceToReplaceWall{ 500.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float HoleSize { 200.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float MinWallPieceHeight{ 50.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float Speed{ 200.f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float Depth{ 200.f };

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mechanics")
	int32 WallsCacheSize{ 3 };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mechanics")
	float HoleBoxScaleMultiplier{ 1.5f };

private:
	bool bIsRunning_{ false };
	float nextHoleHeight_{ 0.f };
	int32 level_{ 0 };
	bool bPieceTypeSemaphore{ false };
};
