#include "FieldSystem/ActivePoint.h"
#include "Characters/Seeker.h"
#include "FieldSystem/ActiveGround.h"
#include "FieldSystem/SavePoint.h"
#include "Components/BoxComponent.h"

AActivePoint::AActivePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());

}

void AActivePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AActivePoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AActivePoint::ActiveAllArea()
{
	IsActivated = true;

	if (ActiveGrounds.Num() > 0)
	{
		for (int i = 0; i < ActiveGrounds.Num(); i++)
		{
			ActiveGrounds[i]->ActivateThisGround();
		}
	}
	
	if (SavePoint)
	{
		SavePoint->IsActivated = true;
		SavePoint->SaveThisPoint();
	}

}

// Called when the game starts or when spawned
void AActivePoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AActivePoint::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AActivePoint::OnOverlapEnd);
	
}

// Called every frame
void AActivePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

