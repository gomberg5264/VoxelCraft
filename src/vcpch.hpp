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
#include "graphics/Texture.hpp"

#include "client/BlockData.hpp"

#include "graphics/TextureAtlas.hpp" // Atlas need blockData
#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderable.hpp"

#include "client/Block.hpp"
#include "client/Chunk.hpp" // Chunk needs Block

#include "graphics/Renderer.hpp" // Renderer needs to know about chunksize

#include "engine/Engine.hpp"