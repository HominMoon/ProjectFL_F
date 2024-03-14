// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Golem.h"
#include "Characters/Seeker.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TargetPoint.h"


// Sets default values
AGolem::AGolem()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RangeCapsule"));
	RangeCapsule->SetupAttachment(GetRootComponent());
	RangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeBox"));
	RangeBox->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AGolem::BeginPlay()
{
	Super::BeginPlay();

	RangeBox->OnComponentBeginOverlap.AddDynamic(this, &AGolem::OnOverlapBegin);
	RangeBox->OnComponentEndOverlap.AddDynamic(this, &AGolem::OnOverlapEnd);

	PlayerOutArea();
	PlayerOutRange();
}

void AGolem::CallInArea()
{
	PlayerInArea();
}

void AGolem::CallOutArea()
{
	PlayerOutArea();
}

void AGolem::CallInRange()
{
	PlayerInRange();
}

void AGolem::CallOutRange()
{
	PlayerOutRange();
}

void AGolem::CallSetPlayer(ASeeker* Player)
{
	Seeker = Player;
	SetPlayer();
}
void AGolem::CallSetBasePoint()
{
	SetBasePoint();
}


// Called every frame
void AGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AGolem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
	if (OtherActor && OtherActor != this && Cast<ASeeker>(OtherActor))
	{
		PlayerInRange();
	}
}

void AGolem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
	if (OtherActor && OtherActor != this)
	{
		//PlayerOutRange();
	}
}



