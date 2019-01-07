#include "ConstraintSpawner.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "Tags.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// SetAngularLimits for Physics Constraints
static FORCEINLINE void SetAngularLimits(
	FConstraintInstance& Constraint,
	const uint8 Swing1Limit, const uint8 Swing2Limit, const uint8 TwistLimit,
	const float Swing1LimitAngle,
	const float Swing2LimitAngle,
	const float TwistLimitAngle,
	bool SoftSwingLimit = true, bool SoftTwistLimit = true,
	const float SwingStiff = 50, const float SwingDamp = 5,
	const float TwistStiff = 50, const float TwistDamp = 5
)
{
	switch (Swing1Limit)
	{
	case 0: Constraint.SetAngularSwing1Motion(ACM_Free);
		break;
	case 1: Constraint.SetAngularSwing1Motion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularSwing1Motion(ACM_Locked);
		break;
	}
	switch (Swing2Limit)
	{
	case 0: Constraint.SetAngularSwing2Motion(ACM_Free);
		break;
	case 1: Constraint.SetAngularSwing2Motion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularSwing2Motion(ACM_Locked);
		break;
	}
	switch (TwistLimit)
	{
	case 0: Constraint.SetAngularTwistMotion(ACM_Free);
		break;
	case 1: Constraint.SetAngularTwistMotion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularTwistMotion(ACM_Locked);
		break;
	}

	// Soft Limit?
	if (SoftSwingLimit) Constraint.ProfileInstance.ConeLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.ConeLimit.bSoftConstraint = 0;

	if (SoftTwistLimit) Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 0;

	// Limit Angles
	Constraint.SetAngularSwing1Limit(ACM_Free, Swing1LimitAngle);
	Constraint.SetAngularSwing2Limit(ACM_Free, Swing2LimitAngle);
	Constraint.SetAngularTwistLimit(ACM_Free, TwistLimitAngle);

	Constraint.ProfileInstance.ConeLimit.Stiffness = SwingStiff;
	Constraint.ProfileInstance.ConeLimit.Damping = SwingDamp;
	Constraint.ProfileInstance.TwistLimit.Stiffness = TwistStiff;
	Constraint.ProfileInstance.TwistLimit.Damping = TwistDamp;
}



// SetLinearLimits for Physics Constraints
static FORCEINLINE void SetLinearLimits(
	FConstraintInstance& Constraint,
	const uint8 XLimit, const uint8 YLimit, const uint8 ZLimit,
	const float Size,
	bool SoftLimit = false,
	const float SoftStiffness = 0,
	const float SoftDampening = 0
)
{
	switch (XLimit)
	{
	case 0: Constraint.SetLinearXMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearXMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearXMotion(LCM_Locked);
		break;
	}
	switch (YLimit)
	{
	case 0: Constraint.SetLinearYMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearYMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearYMotion(LCM_Locked);
		break;
	}
	switch (ZLimit)
	{
	case 0: Constraint.SetLinearZMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearZMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearZMotion(LCM_Locked);
		break;
	}

	Constraint.SetLinearLimitSize(Size);

	// Soft Limit?
	if (SoftLimit) Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 0;

	Constraint.ProfileInstance.LinearLimit.Stiffness = SoftStiffness;
	Constraint.ProfileInstance.LinearLimit.Damping = SoftDampening;
}


bool FConstraintSpawner::SpawnPhysicsConstraintActor(UWorld* World, FString Id, FPhysicsConstraintDetails Details, FVector Location, FRotator Rotator)
{

	UActorComponent* Component = FTags::GetComponentsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Id).Pop();
	if(Component)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]:Component with id:\"%s\" does already exist, so there cannot be an other Constraint with this id"), __FUNCTION__,  *Id);
		return false;
	}

	AActor* First = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Details.IdFirstModel).Pop();
	AActor* Second = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Details.IdSecondModel).Pop();

	if (!First || !Second)
	{
		//at least one of them could not be found.
		if (!First)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:Actor with id:\"%s\" does not exist."), __FUNCTION__, *Details.IdFirstModel);
		}

		if (!Second)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:Actor with id:\"%s\" does not exist."), __FUNCTION__, *Details.IdFirstModel);
		}

		return false;
	}


	//Actors do Exist. Set as ConstraintActors of component. 
	UPhysicsConstraintComponent* ConstraintComponent = NewObject<UPhysicsConstraintComponent>(First);
	ConstraintComponent->bEditableWhenInherited = true;
	ConstraintComponent->ConstraintActor1 = First;
	ConstraintComponent->ConstraintActor2 = Second;

	//Set World Location
	ConstraintComponent->SetWorldLocationAndRotation(Location, Rotator);

	//set up the constraint instance with all the desired values
	FConstraintInstance ConstraintInstance;

	SetupProfileInstance(ConstraintInstance.ProfileInstance, Details);
	SetupLinearLimits(ConstraintInstance, Details.LinearLimits);
	SetupAngularLimits(ConstraintInstance, Details.AngularLimits);

	ConstraintComponent->ConstraintInstance = ConstraintInstance;

	ConstraintComponent->AttachToComponent(First->GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	ConstraintComponent->RegisterComponent();

	// Add Id tag to Component
	FTags::AddKeyValuePair(
		ConstraintComponent,
		TEXT("SemLog"),
		TEXT("id"),
		Id);

	return true;
}


void FConstraintSpawner::SetupProfileInstance(FConstraintProfileProperties& ProfileInstance,
	FPhysicsConstraintDetails Details)
{
	ProfileInstance.bDisableCollision = Details.DisableCollision;
	ProfileInstance.bEnableProjection = Details.EnableProjection;
	ProfileInstance.ProjectionLinearTolerance = Details.ProjectionLinearTolerance;
	ProfileInstance.ProjectionAngularTolerance = Details.ProjectionAngularTolerance;
	ProfileInstance.bParentDominates = Details.ParentDominates;
}


void FConstraintSpawner::SetupAngularLimits(FConstraintInstance& Instance, FAngularLimits AngLimit)
{
	if (AngLimit.UseAdvanced)
	{
		//Advanced features will be set as well.
		SetAngularLimits(Instance,
			AngLimit.Swing1Motion, AngLimit.Swing2Motion, AngLimit.TwistMotion,
			AngLimit.Swing1LimitAngle, AngLimit.Swing2LimitAngle, AngLimit.TwistLimitAngle,
			AngLimit.SwingSoftConstraint, AngLimit.TwistSoftConstraint,
			AngLimit.SwingStiffness, AngLimit.SwingDamping,
			AngLimit.TwistStiffness, AngLimit.TwistDamping
		);

		Instance.AngularRotationOffset = AngLimit.AngularRoationOffset.Rotation();
	}
	else
	{
		//Advanced features will be left default.
		SetAngularLimits(Instance,
			AngLimit.Swing1Motion, AngLimit.Swing2Motion, AngLimit.TwistMotion,
			AngLimit.Swing1LimitAngle, AngLimit.Swing2LimitAngle, AngLimit.TwistLimitAngle);
	}
}

void FConstraintSpawner::SetupLinearLimits(FConstraintInstance& Instance, FLinearLimits LinLimit)
{
	if (LinLimit.UseAdvanced)
	{
		//Advanced features will be set as well.
		SetLinearLimits(Instance,
			LinLimit.XMotion, LinLimit.YMotion, LinLimit.ZMotion,
			LinLimit.Limit,
			LinLimit.SoftConstraint, LinLimit.Stiffness, LinLimit.Damping
		);
	}
	else
	{
		//Advanced features will be left default.
		SetLinearLimits(Instance,
			LinLimit.XMotion, LinLimit.YMotion, LinLimit.ZMotion,
			LinLimit.Limit
		);
	}
}