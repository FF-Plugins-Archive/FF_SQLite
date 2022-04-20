// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"

// Connection Functions.
#include "SQLiteDatabase.h"

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

UCLASS(BlueprintType)
class DB_CONNECTIONS_API USQLite_Connection : public UObject
{
	GENERATED_BODY()

public:

	FSQLiteDatabase* SQLiteDB;
};

UENUM(BlueprintType)
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Open", ToolTip = "Description", Keywords = "sqlite, open"), Category = "DB Connections")
	static bool SQLiteOpen(const FString DB_Path, TEnumAsByte<SQLiteOpenType> OpenType, USQLite_Connection*& OutSQLiteConnection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Close", ToolTip = "Description", Keywords = "sqlite, close"), Category = "DB Connections")
	static void SQLiteClose(USQLite_Connection* InSQLiteConnection);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Columns Names", ToolTip = "Description", Keywords = "sqlite, column, names, get"), Category = "DB Connections")
	static bool SQLiteGetColumnsNames(USQLite_Connection* InSQLiteConnection, const FString TableName, TArray<FString>& OutColumnsNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Single Row Value", ToolTip = "Description", Keywords = "sqlite, column, values, get"), Category = "DB Connections")
	static bool SQLiteGetSingleRowValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString IDColumn, const FString IDIndex, const FString ColumnName, FString& ColumnValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get All Row Values", ToolTip = "Sample: select * from repository where id > 0", Keywords = "sqlite, column, values, get"), Category = "DB Connections")
	static bool SQLiteGetAllRowValues(USQLite_Connection* InSQLiteConnection, const FString Query, const FString ColumnName, TArray<FString>& ColumnValues);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Table Contents", ToolTip = "Description.", Keywords = "sqlite, column, values, get"), Category = "DB Connections")
	static bool SQLiteGetAllTableContents(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString QueryCondition, TMap<FString, FRowValuesStruct>& TableContents);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Write Value", ToolTip = "Description.", Keywords = "sqlite, write, value"), Category = "DB Connections")
	static bool SQLiteWriteValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName, const FString InValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Create Column", ToolTip = "Description.", Keywords = "sqlite, create, column"), Category = "DB Connections")
	static bool SQLiteCreateColumn(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Create Table", ToolTip = "Description.", Keywords = "sqlite, create, table"), Category = "DB Connections")
	static bool SQLiteCreateTable(USQLite_Connection* InSQLiteConnection, const FString TableName);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Helper SQLite Create DB", ToolTip = "Don't use normalized UE4 paths. Use Windows paths such as \\", Keywords = "sqlite, create, database, db"), Category = "DB Connections")
	static FString HelperSQLiteCreateDB(const FString DB_Path, const FString DB_Name);

};