#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESQLiteOpenType : uint8
{
	ReadOnly			UMETA(DisplayName = "ReadOnly"),
	ReadWrite			UMETA(DisplayName = "ReadWrite"),
	ReadWriteCreate		UMETA(DisplayName = "ReadWriteCreate"),
};
ENUM_CLASS_FLAGS(ESQLiteOpenType)