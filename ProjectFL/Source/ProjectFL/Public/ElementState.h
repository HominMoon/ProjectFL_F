#pragma once

UENUM(BlueprintType)
enum class EElementState : uint8
{
	EES_Fire UMETA(DisplayName = "Fire"),
	EES_Water UMETA(DisplayName = "Water"),
	EES_Ground UMETA(DisplayName = "Ground"),
	EES_Grass UMETA(DisplayName = "Grass"),
	EES_Electric UMETA(DisplayName = "Electric"),
	EES_None UMETA(DisplayName = "None")

};
