// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class TPSMELEE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;
	
private:
	/**
	 * Functions
	 */

	/* Combat */
	bool IsActorSameType(AActor* OtherActor);
	void ExecuteGetHit(FHitResult BoxHit);
	void BoxTrace(FHitResult& HitResult);
	
	/**
	* Variables
	*/

	/** Components */
	UPROPERTY(VisibleAnywhere,Category = WeaponProperties)
	TObjectPtr<UBoxComponent> WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

	/* Combat */
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowDebugs = false;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

protected:
	/**
	 * Functions
	 */

	/* Combat */

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	/**
	* Functions
	*/

	/* Inventory */
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);

	/* Other */
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	/**
	 * Variables
	 */

	/* Combat */
	UPROPERTY()
	TArray<AActor*> IgnoredActors;
};
