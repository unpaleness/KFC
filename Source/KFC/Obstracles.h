#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstracles.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogObstracles, Log, All)

class UStaticMeshComponent;

UCLASS()
class AObstracles : public AActor {
	GENERATED_BODY()

public:
	AObstracles();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Roof;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Floor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mechanics")
	float Speed{ 100.f };
};
