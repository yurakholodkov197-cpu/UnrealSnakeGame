// Snake Game. Copyright Yura Kholodkov. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "SG_Grid.generated.h"

namespace Snake
{
class Grid;
}

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
    GENERATED_BODY()

public:
    ASG_Grid();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    /**
     * Set core model to be observed by the grid world actor
     * @param Grid  model object
     * @param CellSize  world size of the cell
     */
    void SetModel(const TSharedPtr<Snake::Grid>& Grid, int32 CellSize);

private:
    Snake::Dim GridDim;
    int32 CellSize;
    int32 WorldWidth;
    int32 WorldHeight;

    void DrawGrid();
};
