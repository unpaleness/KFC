#include "Obstracles.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "KFCGameMode.h"

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

	Floor = CreateDefaultSubobject<UStaticMeshComponent>("Floor");
	if (MeshCube.Succeeded()) {
		Floor->SetStaticMesh(MeshCube.Object);
	}
	Floor->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	for (int32 i = 0; i < WallsCacheSize * 2; ++i) {
		auto Wall = CreateDefaultSubobject<UStaticMeshComponent>(FName(FString::Format(TEXT("WallPiece_{0}"), { i })));
		if (MeshCube.Succeeded()) {
			Wall->SetStaticMesh(MeshCube.Object);
		}
		Wall->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
		WallPieces.Add(MoveTemp(Wall));
	}

	for (int32 i = 0; i < WallsCacheSize; ++i) {
		auto Hole = CreateDefaultSubobject<UBoxComponent>(FName(FString::Format(TEXT("WallHole_{0}"), { i })));
		Hole->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
		WallHoles.Add(MoveTemp(Hole));
	}

	OnActorBeginOverlap.AddDynamic(this, &AObstracles::ProcessHoleBeginOverlap);
}

void AObstracles::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsRunning_) {
		for (auto Wall : WallPieces) {
			Wall->AddRelativeLocation({ 0.f, DeltaTime * Speed, 0.f });
			if (Wall->GetRelativeLocation().Y >= MaxLeftDistanceToReplaceWall) {
				const auto Offset = Wall->GetRelativeLocation().Y - StepWidth * WallsCacheSize;
				const auto NewLocation = bPieceTypeSemaphore ? GetLowerPieceLocation(Offset) : GetUpperPieceLocation(Offset);
				const auto NewScale3D = bPieceTypeSemaphore ? GetLowerPieceScale3D() : GetUpperPieceScale3D();
				Wall->SetRelativeLocation(NewLocation);
				Wall->SetWorldScale3D(NewScale3D);
				bPieceTypeSemaphore = !bPieceTypeSemaphore;
			}
		}
		for (auto Hole : WallHoles) {
			Hole->AddRelativeLocation({ 0.f, DeltaTime * Speed, 0.f });
			if (Hole->GetRelativeLocation().Y >= MaxLeftDistanceToReplaceWall) {
				const auto Offset = Hole->GetRelativeLocation().Y - StepWidth * WallsCacheSize;
				Hole->SetRelativeLocation(GetHoleLocation(Offset));
				Hole->SetWorldScale3D(GetHoleScale3D());
			}
		}
	}
}

void AObstracles::OnConstruction(const FTransform& Transform) {
	Roof->SetRelativeLocation({ 0.f, 0.f, Height / 2.f });
	Roof->SetWorldScale3D({ Depth / 100.f, 16.f, 0.2f });

	Floor->SetRelativeLocation({ 0.f, 0.f, - Height / 2.f });
	Floor->SetWorldScale3D({ Depth / 100.f, 16.f, 0.2f });

	Reset();
}

void AObstracles::OnStartMatch() {
	UE_LOG(LogObstracles, Log, TEXT("Match starts"));

	Reset();

	bIsRunning_ = true;
}

void AObstracles::OnEndMatch() {
	UE_LOG(LogObstracles, Log, TEXT("Match ends"));

	bIsRunning_ = false;
}

void AObstracles::ProcessHoleBeginOverlap(AActor* SelfActor, AActor* OtherActor) {
	if (this != SelfActor || this == OtherActor) {
		return;
	}

	UE_LOG(LogObstracles, Log, TEXT("Hole began overlap with %s"), *OtherActor->GetClass()->GetName());

	++level_;
	RandomizeHoleHieght();
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
	for (int32 i = 0; i < WallsCacheSize; ++i) {
		RandomizeHoleHieght();
		WallPieces[i * 2]->SetRelativeLocation(GetLowerPieceLocation(-DistanceToFirstWall - StepWidth * i));
		WallPieces[i * 2]->SetWorldScale3D(GetLowerPieceScale3D());
		WallPieces[i * 2 + 1]->SetRelativeLocation(GetUpperPieceLocation(-DistanceToFirstWall - StepWidth * i));
		WallPieces[i * 2 + 1]->SetWorldScale3D(GetUpperPieceScale3D());
		WallHoles[i]->SetRelativeLocation(GetHoleLocation(-DistanceToFirstWall - StepWidth * i));
		WallHoles[i]->SetWorldScale3D(GetHoleScale3D());
	}

	level_ = 0;
}

float AObstracles::GetRandomizedHoleHeight() const {
	const auto Range = Height - 2.f * MinWallPieceHeight - HoleSize;
	return FMath::RandRange(- Range / 2.f, Range / 2.f);
}

void AObstracles::RandomizeHoleHieght() {
	nextHoleHeight_ = GetRandomizedHoleHeight();
}

FVector AObstracles::GetLowerPieceLocation(float Offset) const { return { 0.f, Offset, (nextHoleHeight_ - (Height + HoleSize) / 2.f) / 2.f }; }
FVector AObstracles::GetLowerPieceScale3D() const { return { Depth / 100.f, 0.2f, ((Height - HoleSize) / 2.f + nextHoleHeight_) / 100.f }; }
FVector AObstracles::GetUpperPieceLocation(float Offset) const { return { 0.f, Offset, (nextHoleHeight_ + (Height + HoleSize) / 2.f) / 2.f }; }
FVector AObstracles::GetUpperPieceScale3D() const { return { Depth / 100.f, 0.2f, ((Height - HoleSize) / 2.f - nextHoleHeight_) / 100.f }; }
FVector AObstracles::GetHoleLocation(float Offset) const { return { 0.f, Offset, nextHoleHeight_ }; }
FVector AObstracles::GetHoleScale3D() const { return FVector(Depth / 100.f, 0.2f, HoleSize / 100.f) * HoleBoxScaleMultiplier;  }