/*
*	SQLite Path = https://github.com/EpicGames/UnrealEngine/tree/release/Engine/Plugins/Runtime/Database/SQLiteCore/Source/SQLiteCore/Private/sqlite
*/

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes
#include "FF_SQLite_Enums.h"

// UE Includes.
#include "SQLiteDatabase.h"

#include "FF_SQLiteBPLibrary.generated.h"

USTRUCT(BlueprintType)
struct FF_SQLITE_API FSQLiteRowValuesStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Editanywhere)
	TArray<FString> ColumnValues;

};

UCLASS(BlueprintType)
class FF_SQLITE_API USQLite_Connection : public UObject
{
	GENERATED_BODY()

public:

	FSQLiteDatabase* SQLiteDB;
};

UCLASS()
class UFF_SQLiteBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Open", ToolTip = "Description", Keywords = "sqlite, open"), Category = "SQLite")
	static bool SQLiteOpen(const FString DB_Path, ESQLiteOpenType OpenType, USQLite_Connection*& OutSQLiteConnection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Close", ToolTip = "Description", Keywords = "sqlite, close"), Category = "SQLite")
	static void SQLiteClose(USQLite_Connection* InSQLiteConnection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Columns Names", ToolTip = "Description", Keywords = "sqlite, column, names, get"), Category = "SQLite")
	static bool SQLiteGetColumnsNames(USQLite_Connection* InSQLiteConnection, const FString TableName, TArray<FString>& OutColumnsNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Single Row Value", ToolTip = "Description", Keywords = "sqlite, column, values, get"), Category = "SQLite")
	static bool SQLiteGetSingleRowValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString IDColumn, const FString IDIndex, const FString ColumnName, FString& ColumnValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get All Row Values", ToolTip = "Sample: select * from repository where id > 0", Keywords = "sqlite, column, values, get"), Category = "SQLite")
	static bool SQLiteGetAllRowValues(USQLite_Connection* InSQLiteConnection, const FString Query, const FString ColumnName, TArray<FString>& ColumnValues);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Get Table Contents", ToolTip = "Description.", Keywords = "sqlite, column, values, get"), Category = "SQLite")
	static bool SQLiteGetAllTableContents(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString QueryCondition, TMap<FString, FSQLiteRowValuesStruct>& TableContents);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Write Value", ToolTip = "Description.", Keywords = "sqlite, write, value"), Category = "SQLite")
	static bool SQLiteWriteValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName, const FString InValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Create Column", ToolTip = "Description.", Keywords = "sqlite, create, column"), Category = "SQLite")
	static bool SQLiteCreateColumn(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SQLite Create Table", ToolTip = "Description.", Keywords = "sqlite, create, table"), Category = "SQLite")
	static bool SQLiteCreateTable(USQLite_Connection* InSQLiteConnection, const FString TableName);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Helper SQLite Create DB", ToolTip = "Don't use normalized UE4 paths. Use Windows paths such as \\", Keywords = "sqlite, create, database, db"), Category = "SQLite")
	static FString HelperSQLiteCreateDB(const FString DB_Path, const FString DB_Name);

};