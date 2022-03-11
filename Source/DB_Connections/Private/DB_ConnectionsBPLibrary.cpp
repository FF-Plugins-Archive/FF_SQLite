// Copyright Epic Games, Inc. All Rights Reserved.

#include "DB_ConnectionsBPLibrary.h"

// Async Functions.
#include "Async/Async.h"
#include "Async/IAsyncProgress.h"

// Connection Functions.
#include "SQLiteDatabase.h"

#include "DB_Connections.h"

UDB_ConnectionsBPLibrary::UDB_ConnectionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UDB_ConnectionsBPLibrary::SQLiteGetColumnsNames(const FString DB_Path, const FString TableName, TArray<FString>& OutColumnsNames)
{
    FSQLiteDatabase* SQLiteDB = new FSQLiteDatabase();
    SQLiteDB->Open(*DB_Path, ESQLiteDatabaseOpenMode::ReadOnly);

    if (SQLiteDB->IsValid() == true)
    {
        const FString GetColumnNamesQuery = TEXT("select * from ") + TableName;
        FSQLitePreparedStatement LoadStatement;
        LoadStatement.Create(*SQLiteDB, *GetColumnNamesQuery, ESQLitePreparedStatementFlags::Persistent);

        OutColumnsNames = LoadStatement.GetColumnNames();

        LoadStatement.ClearBindings();
        LoadStatement.Destroy();
        SQLiteDB->Close();
        delete SQLiteDB;
    }
}

void UDB_ConnectionsBPLibrary::SQLiteGetSingleRowValue(const FString DB_Path, const FString TableName, const FString IDColumn, const FString IDIndex, const FString ColumnName, FString& ColumnValue)
{
    FSQLiteDatabase* SQLiteDB = new FSQLiteDatabase();
    SQLiteDB->Open(*DB_Path, ESQLiteDatabaseOpenMode::ReadOnly);

    if (SQLiteDB->IsValid() == true)
    {
        const FString GetColumnValuesQuery = TEXT("SELECT * from ") + TableName + TEXT(" WHERE ") + IDColumn + TEXT(" = ") + IDIndex;
        FSQLitePreparedStatement LoadStatement;
        LoadStatement.Create(*SQLiteDB, *GetColumnValuesQuery, ESQLitePreparedStatementFlags::Persistent);

        const FString IDBinding = TEXT("$") + IDColumn;
        LoadStatement.SetBindingValueByName(*IDBinding, IDIndex);
        LoadStatement.Step();
        LoadStatement.GetColumnValueByName(*ColumnName, ColumnValue);

        LoadStatement.ClearBindings();
        LoadStatement.Destroy();
        SQLiteDB->Close();
        delete SQLiteDB;
    }
}

void UDB_ConnectionsBPLibrary::SQLiteGetAllRowValues(const FString DB_Path, const FString Query, const FString ColumnName, TArray<FString>& ColumnValues)
{
    FSQLiteDatabase* SQLiteDB = new FSQLiteDatabase();
    SQLiteDB->Open(*DB_Path, ESQLiteDatabaseOpenMode::ReadOnly);

    if (SQLiteDB->IsValid() == true)
    {
        FSQLitePreparedStatement CustomQueryStatement;
        CustomQueryStatement.Create(*SQLiteDB, *Query, ESQLitePreparedStatementFlags::Persistent);

        FString EachColumnValue;
        while (CustomQueryStatement.Step() == ESQLitePreparedStatementStepResult::Row)
        {
            CustomQueryStatement.GetColumnValueByName(*ColumnName, EachColumnValue);
            ColumnValues.Add(EachColumnValue);
        }

        CustomQueryStatement.ClearBindings();
        CustomQueryStatement.Destroy();
        SQLiteDB->Close();
        delete SQLiteDB;
    }
}

void UDB_ConnectionsBPLibrary::SQLiteGetAllTableContents(const FString DB_Path, const FString TableName, const FString QueryCondition, TMap<FString, FRowValuesStruct>& TableContents)
{
    // Create SQLite query.
    const FString Query = TEXT("SELECT * from ") + TableName + TEXT(" WHERE ") + QueryCondition;

    // Get column names.
    TArray<FString> ColumnNames;
    UDB_ConnectionsBPLibrary::SQLiteGetColumnsNames(DB_Path, TableName, ColumnNames);

    FRowValuesStruct STR_RowValues;
    for (int32 ColumnIndex = 0; ColumnIndex < ColumnNames.Num(); ColumnIndex++)
    {
        TArray<FString> RowValues;
        UDB_ConnectionsBPLibrary::SQLiteGetAllRowValues(DB_Path, Query, ColumnNames[ColumnIndex], RowValues);
        STR_RowValues.ColumnValues = RowValues;

        TableContents.Add(ColumnNames[ColumnIndex], STR_RowValues);
    }
}

