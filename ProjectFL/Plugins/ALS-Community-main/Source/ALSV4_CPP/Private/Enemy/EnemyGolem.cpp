#include "Enemy/EnemyGolem.h"
#include "Components/BoxComponent.h"
#include "AIController.h"

AEnemyGolem::AEnemyGolem()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackBox1 = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox1"));
	AttackBox1->SetupAttachment(GetRootComponent());
}

void AEnemyGolem::OnBox1OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemyGolem::OnBox1OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemyGolem::BeginPlay()
{
	Super::BeginPlay();

	AttackBox1->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGolem::OnBox1OverlapBegin);
	AttackBox1->OnComponentEndOverlap.AddDynamic(this, &AEnemyGolem::OnBox1OverlapEnd);

	MoveToTarget(BasePoint);

}

void AEnemyGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerEngage)
	{

	}
}


