#pragma once

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Unequipped UMETA(DisplayName = "Unequipped"),
	EWS_Equipped UMETA(DisplayName = "Equipped")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Engaged UMETA(DisplayName = "Engaged"),
	ECS_Free UMETA(DisplayName = "Free")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Blocking UMETA(DisplayName = "Blocking"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_Dodge UMETA(DisplayName = "Dodging"),
	EAS_Falling UMETA(DisplayName = "Falling"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ESpeedState : uint8
{
	ESS_Blocking UMETA(DisplayName = "Block Speed"),
	ESS_Running UMETA(DisplayName = "Run Speed"),
	ESS_Sprinting UMETA(DisplayName = "Sprint Speed")
};