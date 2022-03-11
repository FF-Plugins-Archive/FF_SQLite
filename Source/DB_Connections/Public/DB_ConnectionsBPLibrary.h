// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "DB_ConnectionsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM()
enum SQLiteOpenType
{
	ReadOnly			UMETA(DisplayName = "ReadOnly"),
	ReadWrite			UMETA(DisplayName = "ReadWrite"),
	ReadWriteCreate		UMETA(DisplayName = "ReadWriteCreate"),
};

USTRUCT(BlueprintType)
struct FRowValuesStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Editanywhere)
	TArray<FString> ColumnValues;

};

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FSQLiteDelegateAllTable, bool, IsSuccessed);

UCLASS()
class UDB_ConnectionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Columns Names", ToolTip = "Description", Keywords = "sqlite,column,names,get"), Category = "Device Infos")
	static void SQLiteGetColumnsNames(const FString DB_Path, const FString TableName, TArray<FString>& OutColumnsNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Single Row Value", ToolTip = "Description", Keywords = "sqlite,column,values,get"), Category = "Device Infos")
	static void SQLiteGetSingleRowValue(const FString DB_Path, const FString TableName, const FString IDColumn, const FString IDIndex, const FString ColumnName, FString& ColumnValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get All Row Values", ToolTip = "Sample: select * from repository where id > 0", Keywords = "sqlite,column,values,get"), Category = "Device Infos")
	static void SQLiteGetAllRowValues(const FString DB_Path, const FString Query, const FString ColumnName, TArray<FString>& ColumnValues);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Table Contents", ToolTip = "Sample: select * from repository where id > 0", Keywords = "sqlite,column,values,get"), Category = "Device Infos")
	static void SQLiteGetAllTableContents(const FString DB_Path, const FString TableName, const FString QueryCondition, TMap<FString, FRowValuesStruct>& TableContents);

};
