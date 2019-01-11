#include "ConstraintSpawner.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "Tags.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "AssetModifier.h"
#include "Editor.h"

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


	//Check if Id is used already
	TMap<FString, UObject*> IdsToSemLogObjects = FTags::GetKeyValuesToObject(World, TEXT("SemLog"), TEXT("Id"));

	if (IdsToSemLogObjects.Contains(Id))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: Object with id:\"%s\" does already exist, so there cannot be an other Constraint with this id"), *FString(__FUNCTION__), *Id);
		return false;
	}

	AActor** First = (AActor**)IdsToSemLogObjects.Find(Details.IdFirstModel);
	AActor** Second = (AActor**)IdsToSemLogObjects.Find(Details.IdSecondModel);

	if (!First || !Second)
	{
		//at least one of them could not be found.
		if (!First)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Actor with id:\"%s\" does not exist."), *FString(__FUNCTION__), *Details.IdFirstModel);
		}

		if (!Second)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Actor with id:\"%s\" does not exist."), *FString(__FUNCTION__), *Details.IdSecondModel);
		}

		return false;
	}

	FString Label = (*First)->GetActorLabel() + TEXT("_Constraint");

#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Spawning: ") + Label));
#endif

	//Actors do Exist. Set as ConstraintActors of component. 
	APhysicsConstraintActor* ConstraintActor = World->SpawnActor<APhysicsConstraintActor>(Location, Rotator);
	FAssetModifier::AttachModelToParent(*First, ConstraintActor);
	ConstraintActor->SetActorLabel(Label);

	UPhysicsConstraintComponent* ConstraintComponent = ConstraintActor->GetConstraintComp();
	ConstraintComponent->bEditableWhenInherited = true;
	ConstraintComponent->ConstraintActor1 = *First;
	ConstraintComponent->ConstraintActor2 = *Second;

	//Set World Location
	//ConstraintComponent->SetWorldLocationAndRotation(Location, Rotator);

	//set up the constraint instance with all the desired values
	FConstraintInstance ConstraintInstance;

	SetupProfileInstance(ConstraintInstance.ProfileInstance, Details);
	SetupLinearLimits(ConstraintInstance, Details.LinearLimits);
	SetupAngularLimits(ConstraintInstance, Details.AngularLimits);

	ConstraintComponent->ConstraintInstance = ConstraintInstance;
	//ConstraintComponent->SetupAttachment((*First)->GetRootComponent());
	//ConstraintComponent->RegisterComponentWithWorld(World);
	//ConstraintComponent->RegisterComponent();

	//ConstraintComponent->AttachToComponent(ConstraintActor->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));

	// Add Id tag to Component
	FTags::AddKeyValuePair(
		ConstraintActor,
		TEXT("SemLog"),
		TEXT("id"),
		Id);

#if WITH_EDITOR
	ConstraintActor->Modify();
	GEditor->EndTransaction();
#endif

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