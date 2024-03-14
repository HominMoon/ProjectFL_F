// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldSystem/ALSEngageArea.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "Character/ALSCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AALSEngageArea::AALSEngageArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

}

void AALSEngageArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<AALSCharacter>(OtherActor))
	{
		Player = Cast<AALSCharacter>(OtherActor);
		bIsPlayerEnter = true;
		Enemy->IsPlayerEngage = true;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("IsPlayerEngage: %s"), bIsPlayerEnter ? TEXT("true") : TEXT("false")));
		}
	}
}

void AALSEngageArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<AALSCharacter>(OtherActor))
	{
		bIsPlayerEnter = false;
		Enemy->IsPlayerEngage = false;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("IsPlayerEngage: %s"), bIsPlayerEnter ? TEXT("true") : TEXT("false")));
		}
	}
}

// Called when the game starts or when spawned
void AALSEngageArea::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AALSEngageArea::OnOverlapBegin);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AALSEngageArea::OnOverlapEnd);
	
}

// Called every frame
void AALSEngageArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

