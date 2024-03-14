// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldSystem/EngageArea.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Golem.h"
#include "Characters/Seeker.h"


AEngageArea::AEngageArea()
{
	PrimaryActorTick.bCanEverTick = true;
}
