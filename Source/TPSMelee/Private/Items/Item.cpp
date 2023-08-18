// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ItemMesh);

	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

