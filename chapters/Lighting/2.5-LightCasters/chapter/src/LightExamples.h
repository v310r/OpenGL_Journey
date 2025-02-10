#pragma once

#include "Core/Core.h"
#include "Material/Material.h"


struct SCube
{
	STransform Transform;

	std::shared_ptr<Material> MaterialData;
};

int ExecuteDirectionalLightExample();

int ExecutePointLightExample();

int ExecuteSpotlightExample();