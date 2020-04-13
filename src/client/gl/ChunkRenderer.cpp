#include "vcpch.hpp"
#include "common/BlockData.hpp"
#include "common/Chunk.hpp"

#include "client/gl/ChunkRenderer.hpp"

ChunkMesh::ChunkMesh() 
    : m_vao(
        {
            { 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
            { 1, 3, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
            { 2, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 6u * sizeof(float) },
            { 3, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 8u * sizeof(float) },
        })
    , m_elemCount(0)
{
}

bool IsVisible(BlockFace dir, int x, int y, int z, const Chunk& chunk)
{
    // TODO: Please rewrite this. IsVisible is a confusing name for me I like IsOccluded more and
    // make it loop based. Or never touch it again. 

    // Block is only culled if it is surrounded on all sides
    auto& blocks = chunk.GetBlockArray();

    // If we are on an edge
    if ((dir == BlockFace::Left && x == 0) || (dir == BlockFace::Right && x == chunkDimension.x - 1u) ||
        (dir == BlockFace::Bottom && y == 0) || (dir == BlockFace::Top && y == chunkDimension.y - 1u) ||
        (dir == BlockFace::Back && z == 0) || (dir == BlockFace::Front && z == chunkDimension.z - 1u))
    {
        // Just return true if not all neighbors have been loaded yet
        if (chunk.m_neighbors.count != 6) return true;

        // Check if block goes to neighbor
        if (chunk.m_neighbors.neighbor.left && dir == BlockFace::Left && x == 0)
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.left->GetBlockArray()[chunkDimension.x - 1u][y][z]).isSolid;
        else if (chunk.m_neighbors.neighbor.right && dir == BlockFace::Right && x == chunkDimension.x - 1u)
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.right->GetBlockArray()[0][y][z]).isSolid;
        else if (chunk.m_neighbors.neighbor.bottom && dir == BlockFace::Bottom && y == 0)
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.bottom->GetBlockArray()[x][chunkDimension.y - 1u][z]).isSolid;
        else if (chunk.m_neighbors.neighbor.top && dir == BlockFace::Top && y == chunkDimension.y - 1u)
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.top->GetBlockArray()[x][0][z]).isSolid;
        else if (chunk.m_neighbors.neighbor.front && dir == BlockFace::Front && z == chunkDimension.z - 1u)
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.front->GetBlockArray()[x][y][0]).isSolid;
        else if (chunk.m_neighbors.neighbor.back && dir == BlockFace::Back && z == 0 )
            return !BlockDataFactory::GetInstance().GetBlockData(chunk.m_neighbors.neighbor.back->GetBlockArray()[x][y][chunkDimension.z - 1u]).isSolid;
    }

    // If visible from any side we should just render
    if (dir == BlockFace::Right && BlockDataFactory::GetInstance().GetBlockData(blocks[x + 1][y][z]).isSolid) return false;
    else if (dir == BlockFace::Left && BlockDataFactory::GetInstance().GetBlockData(blocks[x - 1][y][z]).isSolid) return false;
    else if (dir == BlockFace::Top && BlockDataFactory::GetInstance().GetBlockData(blocks[x][y + 1][z]).isSolid) return false;
    else if (dir == BlockFace::Bottom && BlockDataFactory::GetInstance().GetBlockData(blocks[x][y - 1][z]).isSolid) return false;
    else if (dir == BlockFace::Front && BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z + 1]).isSolid) return false;
    else if (dir == BlockFace::Back && BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z - 1]).isSolid) return false;

    // This means the face is not occluded
    return true;
}

void ChunkMesh::Generate(const Chunk& chunk)
{
    if (chunk.m_isAir) return;

    Primitive::Face::Buffer mesh;

    const auto& blocks = chunk.GetBlockArray();
    for (unsigned y = 0; y < chunkDimension.y; y++)
        for (unsigned z = 0; z < chunkDimension.z; z++)
            for (unsigned x = 0; x < chunkDimension.x; x++)
            {
                const auto& bData = BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z]);
                if (!bData.isSolid)
                {
                    continue;
                }

                glm::fvec3 bPos(x, y, z);
                bPos += chunk.GetPos();

                // Add all 6 directions
                for (int i = 0; i < BlockFace::Count; i++)
                {
                    if (IsVisible(BlockFace(i), x, y, z, chunk))
                    {
                        auto buffer = Primitive::Face::MakeBuffer(
                            BlockFace(i), bPos.x, bPos.y, bPos.z, bData.texture[i]);

                        mesh.vertices.insert(
                            mesh.vertices.end(),
                            buffer.vertices.begin(),
                            buffer.vertices.end());
                    }
                }
            }

    // Upload to the gpu
    m_vao.m_vbo.Bind();
    m_vao.m_vbo.Upload(mesh.vertices);
    m_elemCount = mesh.vertices.size();
    m_vao.m_vbo.Unbind();
}

ChunkRenderer::ChunkRenderer()
    : m_shader("res/shaders/face.vert","res/shaders/face.frag")
    , m_texture("res/texture.png", 2u, 2u)
{
    // 6 indices per face, 6 faces per voxel
    constexpr auto chunkElemCount = 6u * 6u * chunkSize;

    std::array<unsigned,chunkElemCount> ebo;
    //unsigned ebo[chunkElemCount];
    unsigned offset = 0;
    for (unsigned i = 0; i < chunkElemCount; i += 6)
    {
        unsigned indices[]{ 3 + offset,0 + offset,1 + offset,3 + offset,1 + offset,2 + offset };
        memcpy(&ebo[i], indices, 6 * sizeof(unsigned));
        offset += 4;
    }

    m_ebo.Bind();
    m_ebo.Upload(ebo);
    m_ebo.Unbind();

    //glGenBuffers(1, &m_ebo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementCount * sizeof(unsigned), ebo, GL_STATIC_DRAW);
    //
}

void ChunkRenderer::RegisterEBOToVAO(const VAO& vao)
{
    vao.Bind();
    m_ebo.Bind();
    vao.Unbind();
}

void ChunkRenderer::SetVP(const glm::mat4& vp) noexcept
{
    m_shader.Bind();
    m_shader.SetMatrix("aVP", glm::value_ptr(vp));
}

void ChunkRenderer::SetSkyLightDirection(const glm::vec3& lightDir) noexcept
{
    m_shader.Bind();
    m_shader.SetVec3("aSkyLightDir", glm::value_ptr(glm::max(lightDir,glm::vec3(0))));
}

void ChunkRenderer::SetSkyIntensity(float intensity) noexcept
{
    m_shader.Bind();
    m_shader.SetFloat("aSkyIntensity", intensity);
}

void ChunkRenderer::SetSkyLightColor(const glm::vec3& color) noexcept
{
    m_shader.Bind();
    m_shader.SetVec3("aSkyLightColor", glm::value_ptr(color));
}

void ChunkRenderer::Render(const ChunkMesh& mesh)
{
    m_renderQueue.emplace_back(mesh);
}

void ChunkRenderer::Display() noexcept
{
    m_shader.Bind();
    m_texture.Bind();
    m_shader.SetMatrix("aModel", glm::value_ptr(glm::mat4(1)));

    for (const auto& mesh : m_renderQueue)
    {   
        mesh.get().m_vao.Bind();
        glDrawElements(GL_TRIANGLES, mesh.get().m_elemCount / 4u * 6u, GL_UNSIGNED_INT, 0u);
    }
    m_renderQueue.clear();
}