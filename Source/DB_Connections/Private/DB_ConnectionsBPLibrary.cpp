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

bool UDB_ConnectionsBPLibrary::SQLiteOpen(const FString DB_Path, TEnumAsByte<SQLiteOpenType> OpenType, USQLite_Connection*& OutSQLiteConnection)
{
    ESQLiteDatabaseOpenMode OpenMode = ESQLiteDatabaseOpenMode::ReadOnly;

    switch (OpenType)
    {
    case ReadOnly:
        OpenMode = ESQLiteDatabaseOpenMode::ReadOnly;
        break;

    case ReadWrite:
        OpenMode = ESQLiteDatabaseOpenMode::ReadWrite;
        break;

    case ReadWriteCreate:
        OpenMode = ESQLiteDatabaseOpenMode::ReadWriteCreate;
        break;
    
    default:
        OpenMode = ESQLiteDatabaseOpenMode::ReadOnly;
        break;
    }
    
    USQLite_Connection* SQLiteConnection = NewObject<USQLite_Connection>();
    SQLiteConnection->SQLiteDB = new FSQLiteDatabase();
    SQLiteConnection->SQLiteDB->Open(*DB_Path, OpenMode);
    
    if (SQLiteConnection->SQLiteDB->IsValid() == true)
    {
        OutSQLiteConnection = SQLiteConnection;
        return SQLiteConnection->SQLiteDB->IsValid();
    }

    else
    {
        return SQLiteConnection->SQLiteDB->IsValid();
    }
}

void UDB_ConnectionsBPLibrary::SQLiteClose(USQLite_Connection* InSQLiteConnection)
{
    if (InSQLiteConnection->IsValidLowLevel() == true)
    {
        InSQLiteConnection->SQLiteDB->Close();
        delete InSQLiteConnection->SQLiteDB;
    }
}

bool UDB_ConnectionsBPLibrary::SQLiteGetColumnsNames(USQLite_Connection* InSQLiteConnection, const FString TableName, TArray<FString>& OutColumnsNames)
{
    if (InSQLiteConnection->IsValidLowLevel() == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            const FString GetColumnNamesQuery = TEXT("select * from ") + TableName;
            FSQLitePreparedStatement LoadStatement;
            LoadStatement.Create(*InSQLiteConnection->SQLiteDB, *GetColumnNamesQuery, ESQLitePreparedStatementFlags::Persistent);

            OutColumnsNames = LoadStatement.GetColumnNames();

            LoadStatement.ClearBindings();
            LoadStatement.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

bool UDB_ConnectionsBPLibrary::SQLiteGetSingleRowValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString IDColumn, const FString IDIndex, const FString ColumnName, FString& ColumnValue)
{
    if (InSQLiteConnection->IsValidLowLevel() == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            const FString GetColumnValuesQuery = TEXT("SELECT * from ") + TableName + TEXT(" WHERE ") + IDColumn + TEXT(" = ") + IDIndex;
            FSQLitePreparedStatement LoadStatement;
            LoadStatement.Create(*InSQLiteConnection->SQLiteDB, *GetColumnValuesQuery, ESQLitePreparedStatementFlags::Persistent);

            const FString IDBinding = TEXT("$") + IDColumn;
            LoadStatement.SetBindingValueByName(*IDBinding, IDIndex);
            LoadStatement.Step();
            LoadStatement.GetColumnValueByName(*ColumnName, ColumnValue);

            LoadStatement.ClearBindings();
            LoadStatement.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

bool UDB_ConnectionsBPLibrary::SQLiteGetAllRowValues(USQLite_Connection* InSQLiteConnection, const FString Query, const FString ColumnName, TArray<FString>& ColumnValues)
{
    if (InSQLiteConnection->IsValidLowLevel() == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            FSQLitePreparedStatement CustomQueryStatement;
            CustomQueryStatement.Create(*InSQLiteConnection->SQLiteDB, *Query, ESQLitePreparedStatementFlags::Persistent);

            FString EachColumnValue;
            while (CustomQueryStatement.Step() == ESQLitePreparedStatementStepResult::Row)
            {
                CustomQueryStatement.GetColumnValueByName(*ColumnName, EachColumnValue);
                ColumnValues.Add(EachColumnValue);
            }

            CustomQueryStatement.ClearBindings();
            CustomQueryStatement.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

bool UDB_ConnectionsBPLibrary::SQLiteGetAllTableContents(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString QueryCondition, TMap<FString, FRowValuesStruct>& TableContents)
{
    if (InSQLiteConnection->IsValidLowLevel() == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            // Create SQLite query.
            const FString Query = TEXT("SELECT * from ") + TableName + TEXT(" WHERE ") + QueryCondition;

            // Get column names.
            TArray<FString> ColumnNames;
            UDB_ConnectionsBPLibrary::SQLiteGetColumnsNames(InSQLiteConnection, TableName, ColumnNames);

            FRowValuesStruct STR_RowValues;
            for (int32 ColumnIndex = 0; ColumnIndex < ColumnNames.Num(); ColumnIndex++)
            {
                TArray<FString> RowValues;
                UDB_ConnectionsBPLibrary::SQLiteGetAllRowValues(InSQLiteConnection, Query, ColumnNames[ColumnIndex], RowValues);
                STR_RowValues.ColumnValues = RowValues;

                TableContents.Add(ColumnNames[ColumnIndex], STR_RowValues);
            }

            return true;
        }

        return false;
    }

    return false;
}