#pragma once

// stl 
#include <iostream>
#include <string>

#include <sstream>
#include <fstream>

#include <memory>
#include <utility>

#include <array>
#include <vector>
#include <unordered_map>

// deps
#include <glad/glad.h>

#include <stb_image.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext.hpp>

#include <SFML/Window.hpp>

// proj
#include "graphics/Buffer.hpp"
#include "graphics/Renderable.hpp"

#include "client/BlockData.hpp"
#include "client/Chunk.hpp"

#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"

#include "engine/Engine.hpp"