#include "Obstracles.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogObstracles)

AObstracles::AObstracles() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	Roof = CreateDefaultSubobject<UStaticMeshComponent>("Roof");
	if (MeshCube.Succeeded()) {
		Roof->SetStaticMesh(MeshCube.Object);
	}
	Roof->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Roof->SetRelativeLocation({0.f, 0.f, 300.f});
	Roof->SetRelativeScale3D({0.2f, 16.f, 0.2f});

	Floor = CreateDefaultSubobject<UStaticMeshComponent>("Floor");
	if (MeshCube.Succeeded()) {
		Floor->SetStaticMesh(MeshCube.Object);
	}
	Floor->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Floor->SetRelativeLocation({ 0.f, 0.f, -300.f });
	Floor->SetRelativeScale3D({ 0.2f, 16.f, 0.2f });
}

void AObstracles::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AObstracles::OnConstruction(const FTransform& Transform) {}

void AObstracles::BeginPlay() {
	Super::BeginPlay();
}
