// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Connection Functions.
#include "RemoteDatabaseSupport.h"
#include "RemoteDatabaseConnection.h"

#include "AC_RemoteDB.generated.h"


UCLASS(BlueprintType)
class DB_CONNECTIONS_API URemoteDB_Connection : public UObject
{
	GENERATED_BODY()

public:

	FRemoteDatabaseConnection* RemoteDBConnection;
	FRemoteDataBaseRecordSet* RemoteDBRecordSet;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DB_CONNECTIONS_API UAC_RemoteDB : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_RemoteDB();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoteDB Open", ToolTip = "Description.", Keywords = "remote, database, db, open"), Category = "DB Connections")
	virtual bool RemoteDataBaseOpen(const FString RemoteIP, const FString ConnectionString, const FString StringOverride, URemoteDB_Connection*& OutConnection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoteDB Close", ToolTip = "Description.", Keywords = "remote, database, db, close"), Category = "DB Connections")
	virtual void RemoteDataBaseClose(URemoteDB_Connection* InConnection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoteDB Execute", ToolTip = "Description.", Keywords = "remote, database, db, execute"), Category = "DB Connections")
	virtual bool RemoteDatabaseExecute(URemoteDB_Connection* InConnection, const FString ExecutionString);
};
