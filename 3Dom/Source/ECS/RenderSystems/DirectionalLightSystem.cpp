#include "DirectionalLightSystem.h"

const glm::vec3 DirectionalLightSystem::lightDir = glm::normalize(glm::vec3(20.0f, 50.0f, 20.0f));
unsigned int DirectionalLightSystem::m_lightFrameBuffer = 0; // Initialize with default value
unsigned int DirectionalLightSystem::m_lightDepthMaps = 0; // Initialize with default value
const unsigned int DirectionalLightSystem::m_depthMapResolution = 4096;
unsigned int DirectionalLightSystem::m_matricesUniformBuffer = 0; // Initialize with default value
std::vector<float> DirectionalLightSystem::m_shadowCascadeLevels = {}; // Initialize with default value