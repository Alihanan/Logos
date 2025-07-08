// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereIcosaMeshGenerator.h"

FIcosaPointCoord FIcosaPointCoord::convertPositionToIcosaPoint(FVector position)
{
    return FIcosaPointCoord::convertPositionToIcosaPoint(position, SphereIcosaMeshGenerator::GLOBAL_N_DIVISIONS);
}
