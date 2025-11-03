// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/PhysicsEngine/RadialForceComponent.h>

#include "ActionRoguelike/Public/SMagicProjectile.h"

namespace
{
    constexpr float s_DefaultImpulseStrenght = 2000.0f;
    constexpr float s_DefaultImpulseRadius = 700.0f;
}

ASExplosiveBarrel::ASExplosiveBarrel()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetSimulatePhysics(true);
    StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
    RootComponent = StaticMeshComponent;
    
    RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
    RadialForceComponent->bImpulseVelChange = true;
    RadialForceComponent->ImpulseStrength = s_DefaultImpulseStrenght;
    RadialForceComponent->Radius = s_DefaultImpulseRadius;
    RadialForceComponent->SetupAttachment(RootComponent);
}

void ASExplosiveBarrel::BeginPlay()
{
    Super::BeginPlay();
    
    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnStaticMeshComponentHit);
}

void ASExplosiveBarrel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StaticMeshComponent->OnComponentHit.RemoveDynamic(this, &ASExplosiveBarrel::OnStaticMeshComponentHit);

    Super::EndPlay(EndPlayReason);
}

void ASExplosiveBarrel::OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    check(OtherActor != nullptr);
    
    if (OtherActor->IsA<ASMagicProjectile>())
    {
        RadialForceComponent->FireImpulse();
        OtherActor->Destroy();
        Destroy();
    }
}
