// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_RemoteDB.h"

// Connection Functions.
#include "RemoteDatabaseSupport.h"
#include "RemoteDatabaseConnection.h"

// Sets default values for this component's properties
UAC_RemoteDB::UAC_RemoteDB()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_RemoteDB::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAC_RemoteDB::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAC_RemoteDB::RemoteDataBaseOpen(const FString RemoteIP, const FString ConnectionString, const FString RemoteConnectionStringOverride)
{
	return UAC_RemoteDB::RemoteDBConnection->Open(*RemoteIP, *ConnectionString, *RemoteConnectionStringOverride);
}