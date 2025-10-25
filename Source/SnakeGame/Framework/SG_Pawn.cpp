// Snake Game. Copyright Yura Kholodkov. All Rights Reserved.

#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
float HalfFOVTan(float FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5f));
}

float VerticalFOV(float HorFOVDegrees, float ViewportAspectHW)
{
    return FMath::RadiansToDegrees  //
        (2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorFOVDegrees) * 0.5f) * ViewportAspectHW));
}

}  // namespace

ASG_Pawn::ASG_Pawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetupAttachment(GetRootComponent());
    Camera->AddRelativeRotation(FRotator(-90, 0, 0));
    Camera->bOverrideAspectRatioAxisConstraint = true;
}

void ASG_Pawn::UpdateLocation(const Snake::Dim& InDim, int32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Pawn::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.height == 0) return;

    const float WorldWidth = Dim.width * CellSize;
    const float WorldHeight = Dim.height * CellSize;
    float LocationZ = 0.0f;
    const float ViewportAspect = static_cast<float>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const float GridAspect = static_cast<float>(Dim.width) / Dim.height;

    if (ViewportAspect <= GridAspect)
    {
        LocationZ = WorldWidth * 0.5f / HalfFOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const float VFOV = VerticalFOV(Camera->FieldOfView, 1.0f / ViewportAspect);
        LocationZ = WorldHeight * 0.5f / HalfFOVTan(VFOV);
    }

    float CameraHeightMultiplier = 1.15f;
    const FVector NewPawnLocation =
        GridOrigin.GetLocation() + FVector(0.5f * WorldHeight, 0.5f * WorldWidth, CameraHeightMultiplier * LocationZ);
    SetActorLocation(NewPawnLocation);
}