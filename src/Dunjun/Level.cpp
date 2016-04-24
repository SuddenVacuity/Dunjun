
#include <Dunjun/Level.hpp>

namespace Dunjun
{

	Level::Level()
	{

	}

	void Level::generate()
	{
		if (!mesh)
			mesh = new Mesh();

		mesh->addData(m_meshData);
	}

	Level::~Level()
	{
		delete mesh;
	}

	void Level::addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos)
	{
		if (!mesh)
			mesh = new Mesh();

		// size of image map
		const f32 tileWidth = 1.0f / 16;
		const f32 tileHeight = 1.0f / 16;

		size_t index = m_meshData.vertices.size();

		//right/left vertices
		if ((u32)face < 2)
		{
			//vertex data				position (x, y, z}									texture coords {s, t}									vertex colors{r, g, b, a}
			 m_meshData.vertices.push_back({ { position.x, position.y + 1.0f, position.z + 1.0f },{ (texPos.x + 1)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x, position.y + 1.0f, position.z + 0.0f },{ (texPos.x + 0)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x, position.y + 0.0f, position.z + 1.0f },{ (texPos.x + 1)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x, position.y + 0.0f, position.z + 0.0f },{ (texPos.x + 0)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
		}

		// up/down vertices
		else if ((u32)face < 4)
		{
			//vertex data				position (x, y, z}									texture coords {s, t}									vertex colors{r, g, b, a}
			 m_meshData.vertices.push_back({ { position.x + 1.0f, position.y, position.z + 0.0f },{ (texPos.x + 1)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 0.0f, position.y, position.z + 0.0f },{ (texPos.x + 0)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 1.0f, position.y, position.z + 1.0f },{ (texPos.x + 1)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 0.0f, position.y, position.z + 1.0f },{ (texPos.x + 0)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
		}

		// forward/backward vertices
		else if ((u32)face< 6)
		{
			//vertex data				position (x, y, z}									texture coords {s, t}									vertex colors{r, g, b, a}
			 m_meshData.vertices.push_back({ { position.x + 1.0f, position.y + 1.0f, position.z },{ (texPos.x + 1)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 0.0f, position.y + 1.0f, position.z },{ (texPos.x + 0)*tileWidth, (texPos.y + 1)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 1.0f, position.y + 0.0f, position.z },{ (texPos.x + 1)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
			 m_meshData.vertices.push_back({ { position.x + 0.0f, position.y + 0.0f, position.z },{ (texPos.x + 0)*tileWidth, (texPos.y + 0)*tileHeight },{ 0xFF, 0xFF, 0xFF, 0xFF } });
		}
		else
			std::runtime_error(std::string("Invalid tile direction in function: addTileSurface()"));

		// choose draw order based on direction
		if ((u32)face % 2 == 0)
		{
			 m_meshData.indices.push_back(index + 0);
			 m_meshData.indices.push_back(index + 1);
			 m_meshData.indices.push_back(index + 2);
			 m_meshData.indices.push_back(index + 1);
			 m_meshData.indices.push_back(index + 3);
			 m_meshData.indices.push_back(index + 2);
		}
		else // (u32)face % 2 == 1
		{
			 m_meshData.indices.push_back(index + 2);
			 m_meshData.indices.push_back(index + 3);
			 m_meshData.indices.push_back(index + 1);
			 m_meshData.indices.push_back(index + 2);
			 m_meshData.indices.push_back(index + 1);
			 m_meshData.indices.push_back(index + 0);
		}
	}

	void Level::addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTilePosSet)
	{
		size_t length = randomTilePosSet.size();
		TileId tilePos = randomTilePosSet[rand() % length];
		addTileSurface(position, face, tilePos);
	}


} // end Dunjun
