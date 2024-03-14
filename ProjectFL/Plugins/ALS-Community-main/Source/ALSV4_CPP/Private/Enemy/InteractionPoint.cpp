#include "Enemy/InteractionPoint.h"
#include "Components/BoxComponent.h"
#include "Character/ALSCharacter.h"

AInteractionPoint::AInteractionPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Box);

	Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionPoint::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AInteractionPoint::OnOverlapEnd);

}

void AInteractionPoint::PlayerRequestInteract()
{

}

void AInteractionPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && Cast<AALSCharacter>(OtherActor))
	{
		Player = Cast<AALSCharacter>(OtherActor);
		Player->SetOverlappingPoint(this);
		UE_LOG(LogTemp, Warning, TEXT("OverlapBegin %s"), *GetName());
	}
}

void AInteractionPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && Cast<AALSCharacter>(OtherActor))
	{
		Player->SetOverlappingPoint(nullptr);
		Player = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("OverlapEnd %s"), *GetName());
	}
}

