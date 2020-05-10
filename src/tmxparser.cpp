#include <sstream>

#include "tinyxml2.h"
#include "tmxparser.h"

using namespace tinyxml2;

const unsigned tmxparser::FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned tmxparser::FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned tmxparser::FLIPPED_DIAGONALLY_FLAG = 0x20000000;

TMXData tmxparser::parseTileMap(const std::string& fileName)
{
	TMXData data;

	// Load the map document
	XMLDocument mapDoc;
	mapDoc.LoadFile(fileName.c_str());
	
	// Get the map node and get the map size and tile sizes from it
	XMLElement* mapNode = mapDoc.FirstChildElement("map");
	mapNode->QueryIntAttribute("width", &(data.mapWidth));
	mapNode->QueryIntAttribute("height", &(data.mapHeight));
	mapNode->QueryIntAttribute("tilewidth", &(data.tileWidth));
	mapNode->QueryIntAttribute("tileheight", &(data.tileHeight));

	// Get the first tileset node
	XMLElement* tileSetNode = mapNode->FirstChildElement("tileset");
	if (tileSetNode)
	{
		// Loop through the tileset elements and add them to tileSets
		while (tileSetNode)
		{
			XMLDocument tileSetDoc;
			TMXTileSet tileSet;
			// Get first GID for the tileset
			tileSetNode->QueryIntAttribute("firstgid", &(tileSet.firstGID));

			// Get the tileset file source and load it
			const char* tilesetSrc = tileSetNode->Attribute("source");
			std::string tilesetSrcStr(tilesetSrc);
			// Add the full folder path to the string
			tilesetSrcStr = fileName.substr(0, fileName.rfind('/') + 1) + tilesetSrcStr;
			tileSetDoc.LoadFile(tilesetSrcStr.c_str());

			// Get tileset data from tileset node in tileset file
			XMLElement* tNode = tileSetDoc.FirstChildElement("tileset");
			tNode->QueryIntAttribute("tilewidth", &(tileSet.tileWidth));
			tNode->QueryIntAttribute("tileheight", &(tileSet.tileHeight));
			tNode->QueryIntAttribute("spacing", &(tileSet.spacing));

			XMLElement* imageNode = tNode->FirstChildElement("image");
			const char* imgSrc = imageNode->Attribute("source");
			std::string imgSrcStr(imgSrc);
			// erase all the "../"
			int index = imgSrcStr.find("../");
			while (index != std::string::npos)
			{
				imgSrcStr = imgSrcStr.erase(index, 3);
				index = imgSrcStr.find("../");
			}
			// Add the first folder path to the string
			imgSrcStr = fileName.substr(0, fileName.find('/') + 1) + imgSrcStr;
			tileSet.source = imgSrcStr;
			// image width and height
			imageNode->QueryIntAttribute("width", &(tileSet.width));
			imageNode->QueryIntAttribute("height", &(tileSet.height));

			data.tileSets.push_back(tileSet);
			
			// get the next tileset
			tileSetNode = tileSetNode->NextSiblingElement("tileset");
		}

		// Loop through layers
		XMLElement* layerNode = mapNode->FirstChildElement("layer");
		while (layerNode)
		{
			// width and height in tiles
			int layerWidth, layerHeight;
			layerNode->QueryIntAttribute("width", &layerWidth);
			layerNode->QueryIntAttribute("height", &layerHeight);

			// Initialize grid of tiles
			std::vector<std::vector<TMXTile>> tileGrid;
			tileGrid.reserve(layerHeight);
			for (int i = 0; i < layerHeight; i++)
			{
				std::vector<TMXTile> row;
				row.reserve(layerWidth);
				tileGrid.push_back(row);
			}
			
			XMLElement* dataNode = layerNode->FirstChildElement("data");
			std::string tileCSV = dataNode->GetText();

			int row = 0, col = 0;

			// Break up the CSV into tokens
			std::stringstream ss(tileCSV);
			std::string token;
			while (std::getline(ss, token, ','))
			{
				TMXTile tile;
				// Convert tokens to int gid
				unsigned gidWithFlags = 0;
				try {
					gidWithFlags = stoul(token);
				}
				catch (std::invalid_argument) {}
				catch (std::out_of_range) {}

				// Read out the flags
				tile.flippedHorizontally = (gidWithFlags & FLIPPED_HORIZONTALLY_FLAG);
				tile.flippedVertically = (gidWithFlags & FLIPPED_VERTICALLY_FLAG);
				tile.flippedDiagonally = (gidWithFlags & FLIPPED_DIAGONALLY_FLAG);

				// Clear the flags and store the gid
				tile.gid = gidWithFlags & ~(FLIPPED_HORIZONTALLY_FLAG |
					FLIPPED_VERTICALLY_FLAG |
					FLIPPED_DIAGONALLY_FLAG);

				for (int i = data.tileSets.size() - 1; i >= 0; i--)
				{
					if (data.tileSets.at(i).firstGID <= tile.gid)
					{
						tile.tileSetId = i;
						break;
					}
				}

				// Make sure we're not pushing extra tiles
				if (row < layerHeight)
				{
					tileGrid.at(row).push_back(tile);
				}

				if (col < layerWidth - 1)
				{
					col++;
				}
				else
				{
					row++;
					col = 0;
				}
			}

			// Make sure the grid is full
			while (row < layerHeight)
			{
				tileGrid.at(row).push_back(TMXTile());

				if (col < layerWidth - 1)
				{
					col++;
				}
				else
				{
					row++;
					col = 0;
				}
			}

			// Add layer's grid
			data.tileGridLayers.push_back(tileGrid);

			layerNode = layerNode->NextSiblingElement("layer");
		}
	}

	return data;
}