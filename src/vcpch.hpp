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
#include "client/BlockData.hpp"
#include "client/Chunk.hpp"

#include "client/gl/Primitive.hpp"
#include "client/gl/Shader.hpp"
#include "client/gl/Camera.hpp"

#include "client/gl/renderer/Window.hpp"
#include "client/gl/renderer/ChunkRenderer.hpp"

#include "engine/Engine.hpp"