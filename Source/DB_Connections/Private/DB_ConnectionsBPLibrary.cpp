// Copyright Epic Games, Inc. All Rights Reserved.

#include "DB_ConnectionsBPLibrary.h"
#include "DB_Connections.h"

// Async Functions.
#include "Async/Async.h"
#include "Async/IAsyncProgress.h"

// Connection Functions.
#include "SQLiteDatabase.h"

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
    if (IsValid(InSQLiteConnection) == true)
    {
        InSQLiteConnection->SQLiteDB->Close();
        delete InSQLiteConnection->SQLiteDB;
    }
}

bool UDB_ConnectionsBPLibrary::SQLiteGetColumnsNames(USQLite_Connection* InSQLiteConnection, const FString TableName, TArray<FString>& OutColumnsNames)
{
    if (IsValid(InSQLiteConnection) == true)
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
    if (IsValid(InSQLiteConnection) == true)
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
    if (IsValid(InSQLiteConnection) == true)
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
    if (IsValid(InSQLiteConnection) == true)
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

bool UDB_ConnectionsBPLibrary::SQLiteWriteValue(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName, const FString InValue)
{
    if (IsValid(InSQLiteConnection) == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            const FString WriteQuery = TEXT("INSERT INTO ") + TableName + TEXT(" (") + ColumnName + TEXT(") VALUES ('") + InValue + TEXT("')");
            
            FSQLitePreparedStatement StatementWriteValue;
            StatementWriteValue.Reset();
            StatementWriteValue.Create(*InSQLiteConnection->SQLiteDB, *WriteQuery, ESQLitePreparedStatementFlags::Persistent);
            StatementWriteValue.SetBindingValueByName(*ColumnName, *InValue);
            StatementWriteValue.Execute();

            StatementWriteValue.ClearBindings();
            StatementWriteValue.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

bool UDB_ConnectionsBPLibrary::SQLiteCreateColumn(USQLite_Connection* InSQLiteConnection, const FString TableName, const FString ColumnName)
{
    if (IsValid(InSQLiteConnection) == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            const FString QueryCreateColumn = TEXT("ALTER TABLE ") + TableName + TEXT(" ADD ") + ColumnName + TEXT(" TEXT");
            
            FSQLitePreparedStatement StatementCreateColumn;
            StatementCreateColumn.Reset();
            StatementCreateColumn.Create(*InSQLiteConnection->SQLiteDB, *QueryCreateColumn, ESQLitePreparedStatementFlags::Persistent);
            StatementCreateColumn.Execute();

            StatementCreateColumn.ClearBindings();
            StatementCreateColumn.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

bool UDB_ConnectionsBPLibrary::SQLiteCreateTable(USQLite_Connection* InSQLiteConnection, const FString TableName)
{
    if (IsValid(InSQLiteConnection) == true)
    {
        if (InSQLiteConnection->SQLiteDB->IsValid() == true)
        {
            const FString QueryCreateTable = TEXT("CREATE TABLE IF NOT EXISTS ") + TableName + TEXT(" (id INTEGER PRIMARY KEY)");
            
            FSQLitePreparedStatement StatementCreateTable;
            StatementCreateTable.Reset();
            StatementCreateTable.Create(*InSQLiteConnection->SQLiteDB, *QueryCreateTable, ESQLitePreparedStatementFlags::Persistent);
            StatementCreateTable.Execute();

            StatementCreateTable.ClearBindings();
            StatementCreateTable.Destroy();

            return true;
        }

        return false;
    }

    return false;
}

FString UDB_ConnectionsBPLibrary::HelperSQLiteCreateDB(const FString DB_Path, const FString DB_Name)
{
    return TEXT("sqlite3 ") + DB_Path + DB_Name + TEXT(".db") + TEXT(" \"VACCUUM;\"");
}