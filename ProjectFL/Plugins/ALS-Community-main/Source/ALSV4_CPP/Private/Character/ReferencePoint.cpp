// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ReferencePoint.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Character/ALSCharacter.h"

// Sets default values
AReferencePoint::AReferencePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Point"));
	SetRootComponent(Capsule);

	Capsule->SetCapsuleRadius(1.f);
	Capsule->SetCapsuleHalfHeight(1.f);*/

}

// Called when the game starts or when spawned
void AReferencePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReferencePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

