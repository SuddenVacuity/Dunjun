
#include <Dunjun/Level.hpp>

#include <random>

namespace Dunjun
{

	Level::Level()
	{

	}

	void Level::generate()
	{
		if (!mesh)
			mesh = new Mesh();

		u32 roomSizeX = 100;
		u32 roomSizeZ = 5;
		u32 roomSizeY = 4;
		u32 wallHeight = 1;

		// initialize mapGrid
		mapGrid = std::vector<std::vector<TileId>>(roomSizeX, std::vector<TileId>(roomSizeZ));

		static const TileId emptyTile = {0xFFFFFFFF, 0xFFFFFFFF};

		std::random_device seed; // random seed
		std::mt19937 randNum(1); // random number generator
		std::uniform_int_distribution<int> randRange(0, 100); // range of random number

		// location of texture in image map
		Level::TileId darkBrownDirtTile = { 3, 14 };
		Level::TileId lightLogsTile = { 1, 11 };
		Level::TileId lightLogEndTile = { 2, 11 };
		Level::TileId stoneTile = { 2, 15 };
		
		Level::RandomTileSet mossyStoneTiles;
		for (u32 i = 1; i <= 2; i++)
			mossyStoneTiles.emplace_back(Level::TileId{ i, 15 });

		// set texture in mapGrid
		for (int i = 0; i < roomSizeX; i++)
			for (int j = 0; j < roomSizeZ; j++)
				if(randRange(randNum) > 10) // randomized generation
					mapGrid[i][j] = { 0, 11 };
				else
					mapGrid[i][j] = emptyTile;

		// generate mesh
		for (int i = 0; i < roomSizeX; i++)
			for (int j = 0; j < roomSizeZ; j++)
			{ 
				// generate edge walls
				if (i == 0)
				{ // left edge
					for (int k = 0; k < roomSizeY; k++)
						if(mapGrid[i][j] != emptyTile || k > wallHeight - 1) // only add a wall if there's a floor or if is higher than obstacle
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Right, mossyStoneTiles);
				}
				else if (i == roomSizeX - 1)
				{ // right edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Left, mossyStoneTiles);
				}
				if (j == 0)
				{ // back edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Forward, mossyStoneTiles);
				}
				else if (j == roomSizeZ - 1)
				{ // front edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Backward, mossyStoneTiles);
				}
				

				// generate floor and internal walls
				if(mapGrid[i][j] != emptyTile)
				{ // generate floors
					addTileSurface(Vector3(i, 0, j), TileSurfaceFace::Up, darkBrownDirtTile);
				}
				else
				{ // generate internal walls
					for(int k = 0; k < wallHeight; k++)
					{ // loop for mapSizeY for height
						if (i > 0)
							if (mapGrid[i - 1][j] != emptyTile)
								addTileSurface(Vector3(i, k, j), TileSurfaceFace::Left, lightLogsTile);
							
						if (i < roomSizeX - 1)
							if (mapGrid[i + 1][j] != emptyTile)
								addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Right, lightLogsTile);

						if (j > 0)
							if (mapGrid[i][j - 1] != emptyTile)
								addTileSurface(Vector3(i, k, j), TileSurfaceFace::Backward, lightLogsTile);
						if (j < roomSizeZ - 1)
							if (mapGrid[i][j + 1] != emptyTile)
								addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Forward, lightLogsTile);
					}

					// cap tops of walls
					addTileSurface(Vector3(i, wallHeight, j), TileSurfaceFace::Up, lightLogEndTile);
				} // end generate internal walls

				// generate ceiling
				addTileSurface(Vector3(i, roomSizeY, j), TileSurfaceFace::Down, mossyStoneTiles);

			} // end generate floors and internal walls

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
