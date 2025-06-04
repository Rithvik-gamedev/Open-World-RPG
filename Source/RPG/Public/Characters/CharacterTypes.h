#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped Two Handed")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReacting UMETA(DisplayName = "Hit Reacting"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_DeathPose1 UMETA(DisplayName = "Death Pose 1"),
	EDP_DeathPose2 UMETA(DisplayName = "Death Pose 2"),
	EDP_DeathPose3 UMETA(DisplayName = "Death Pose 3"),
	EDP_DeathPose4 UMETA(DisplayName = "Death Pose 4"),
	EDP_DeathPose5 UMETA(DisplayName = "Death Pose 5"),
	EDP_DeathPose6 UMETA(DisplayName = "Death Pose 6"),
	EDP_DeathPose7 UMETA(DisplayName = "Death Pose 7"),

	EDP_DefaultMax UMETA(DisplayName = "DefaultMAX")
};

UENUM(Blueprinttype)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "No State"),
	
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")

};