#include "Enemy/WeakPoint.h"
//#include "Enemy/EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Character/ALSCharacter.h"

AWeakPoint::AWeakPoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeakPoint::PlayerRequestInteract()
{
	Super::PlayerRequestInteract();

	if (Player->IsAttackCharged)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!"));
		DestroyThisPoint();
	}

	
}

void AWeakPoint::DestroyThisPoint()
{
	if (WasDestroyed) return;

	//animation, sound
	//Base->Health -= 1;
	/*if (Base->Health == 0)
	{
		Base->Eleminate();
	}*/
	WasDestroyed = true;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeakPoint::BeginPlay()
{
	Super::BeginPlay();
	//Base = CastChecked<AEnemyBase>(GetAttachParentActor());
}

// Called every frame
void AWeakPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

