

#include "FieldSystem/SavePoint.h"
#include "Characters/Seeker.h"
#include "Components/BoxComponent.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());

}

void ASavePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<ASeeker>(OtherActor))
	{
		ASeeker* Player = Cast<ASeeker>(OtherActor);
		Player->LastSavePoint = this;

		if (IsActivated)
		{
			SaveThisPoint();
		}
	}
}

void ASavePoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnOverlapBegin);
	
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

