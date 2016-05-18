
#include <Dunjun/Level/Room.hpp>

namespace Dunjun
{
	Room::Room(Random& random, const Room::Size& size)
		: SceneNode()
		, size(size)
		, material(nullptr)
		, m_mesh(nullptr)
		, m_random(random)
		, m_generated(false)
		, m_meshData()
	{
	}

	Room::~Room()
	{
		delete m_mesh;
	}

	void Room::generate()
	{
		if(m_generated)
			return;

		if(!m_mesh)
			m_mesh = new Mesh();

		std::vector<std::vector<TileId>> mapGrid(size.x, std::vector<TileId>(size.z));

		static const TileId emptyTile = { 0xFFFFFFFF, 0xFFFFFFFF };

		// location of texture in image map
		TileId darkBrownDirtTile = { 3, 14 };
		TileId lightWoodTile = { 0, 11 };
		TileId lightLogsTile = { 1, 11 };
		TileId lightLogEndTile = { 2, 11 };
		TileId stoneTile = { 2, 15 };
		TileId whiteTile = { 15, 15 };

		RandomTileSet mossyStoneTiles;
		for (u32 i = 1; i <= 2; i++)
			mossyStoneTiles.emplace_back(TileId{ i, 15 });

		// initialize mapGrid to build room from
		for (int i = 0; i < size.x; i++)
		{
			for (int j = 0; j < size.z; j++)
				mapGrid[i][j] = emptyTile;
		}

		for (int i = 0; i < size.x; i++)
		{
			for (int j = 0; j < size.z; j++)
			{
				//if(m_random.getBool() == true)
					mapGrid[i][j] = lightWoodTile;
			}
		}

		// generate mesh
		for (int i = 0; i < size.x; i++)
			for (int j = 0; j < size.z; j++)
			{
				// generate edge walls
				if (i == 0)
				{ // left edge
					for (int k = 0; k < size.y; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0) // only add a wall if there's a floor or if is higher than obstacle
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Right, mossyStoneTiles);
				}
				else if (i == size.x - 1)
				{ // right edge
					for (int k = 0; k < size.y; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Left, mossyStoneTiles);
				}
				if (j == 0)
				{ // back edge
					for (int k = 0; k < size.y; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Backward, mossyStoneTiles);
				}
				else if (j == size.z - 1)
				{ // front edge
					for (int k = 0; k < size.y; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Forward, mossyStoneTiles);
				}


				// generate floor and internal walls
				if (mapGrid[i][j] != emptyTile)
				{ // generate floors
					addTileSurface(Vector3(i, 0, j), TileSurfaceFace::Up, darkBrownDirtTile);
				}
				// generate ceiling
				addTileSurface(Vector3(i, size.y, j), TileSurfaceFace::Down, mossyStoneTiles);

			} // end generate floors and internal walls

		m_mesh->addData(m_meshData);

		addComponent<MeshRenderer>(*m_mesh, *material);

		m_generated = true;
	}


	// create a 4 vertex tile in the at position in the direction of TileSurfaceFace with texPos being the position in texture map for the texture
	void Room::addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos)
	{
		//if (!m_mesh)
		//	m_mesh = new Mesh();

		// size of image map
		const f32 tileWidth = 1.0f / 16;
		const f32 tileHeight = 1.0f / 16;

		// used to offset indices
		size_t index = m_meshData.vertices.size();

		// predefine this to save space and allow value swapping
		Vector2 vertexTexCoord[4] = { { (texPos.x + 1)*tileWidth, (texPos.y + 1)*tileHeight },
									  { (texPos.x + 0)*tileWidth, (texPos.y + 1)*tileHeight },
									  { (texPos.x + 1)*tileWidth, (texPos.y + 0)*tileHeight },
									  { (texPos.x + 0)*tileWidth, (texPos.y + 0)*tileHeight } };

		// predefine this to save space and allow value swapping
		Color vertexColor[4] = { { 0xFF, 0xFF, 0xFF, 0xFF },
								 { 0xFF, 0xFF, 0xFF, 0xFF },
								 { 0xFF, 0xFF, 0xFF, 0xFF },
								 { 0xFF, 0xFF, 0xFF, 0xFF } };

		// choose draw order based on direction
		if ((u32)face % 2 == 0)
			m_meshData.addFace(index, 0, 1, 2).addFace(index, 1, 3, 2); // hypotenuse goes down and right
		else // (u32)face % 2 == 1
		{
			// flip draw order
			m_meshData.addFace(index, 0, 2, 3).addFace(index, 0, 3, 1); // hypotenuse goes down and right

			// make temp variables and flip texture/colors to match draw order
			Vector2 v[4] = { vertexTexCoord[1], vertexTexCoord[0], vertexTexCoord[3], vertexTexCoord[2] };
			Color c[4] = { vertexColor[1], vertexColor[0], vertexColor[3], vertexColor[2] };

			// apply the flipped coords and colors
			for (int i = 0; i < 4; i++)
			{
				vertexTexCoord[i] = v[i];
				vertexColor[i] = c[i];
			}
		}

		// apply position portions based on direction
		// right/left vertices
		if ((u32)face < 2)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			m_meshData.vertices.append({ position.x, position.y + 1.0f, position.z + 1.0f }, vertexTexCoord[0], vertexColor[0]);
			m_meshData.vertices.append({ position.x, position.y + 1.0f, position.z + 0.0f }, vertexTexCoord[1], vertexColor[1]);
			m_meshData.vertices.append({ position.x, position.y + 0.0f, position.z + 1.0f }, vertexTexCoord[2], vertexColor[2]);
			m_meshData.vertices.append({ position.x, position.y + 0.0f, position.z + 0.0f }, vertexTexCoord[3], vertexColor[3]);
		}

		// up/down vertices
		else if ((u32)face < 4)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			m_meshData.vertices.append({ position.x + 1.0f, position.y, position.z + 0.0f }, vertexTexCoord[0], vertexColor[0]);
			m_meshData.vertices.append({ position.x + 0.0f, position.y, position.z + 0.0f }, vertexTexCoord[1], vertexColor[1]);
			m_meshData.vertices.append({ position.x + 1.0f, position.y, position.z + 1.0f }, vertexTexCoord[2], vertexColor[2]);
			m_meshData.vertices.append({ position.x + 0.0f, position.y, position.z + 1.0f }, vertexTexCoord[3], vertexColor[3]);
		}

		// forward/backward vertices
		else if ((u32)face< 6)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			m_meshData.vertices.append({ position.x + 1.0f, position.y + 1.0f, position.z }, vertexTexCoord[0], vertexColor[0]);
			m_meshData.vertices.append({ position.x + 0.0f, position.y + 1.0f, position.z }, vertexTexCoord[1], vertexColor[1]);
			m_meshData.vertices.append({ position.x + 1.0f, position.y + 0.0f, position.z }, vertexTexCoord[2], vertexColor[2]);
			m_meshData.vertices.append({ position.x + 0.0f, position.y + 0.0f, position.z }, vertexTexCoord[3], vertexColor[3]);
		}
		else
			std::runtime_error(std::string("Invalid tile direction in function Room::addTileSurface()"));

	}

	void Room::addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTilePosSet)
	{
		size_t length = randomTilePosSet.size();
		TileId tilePos = randomTilePosSet[m_random.getInt(0, length - 1)];
		addTileSurface(position, face, tilePos);
	}


} // end Dunjun
