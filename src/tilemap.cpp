#include "tilemap.h"
#include "graphics.h"
#include "tmxparser.h"
#include "rectangle.h"

TileMap::TileMap(Graphics &graphics, const std::string& tmxFileName)
{
	TMXData data = tmxparser::parseTileMap(tmxFileName);
	for (std::vector<std::vector<TMXTile>> tmxLayer : data.tileGridLayers)
	{
		std::vector<std::vector<Tile>> layer;
		
		for (std::vector<TMXTile> tmxRow : tmxLayer)
		{
			std::vector<Tile> row;

			for (TMXTile tmxTile : tmxRow)
			{
				Tile tile;
				tile.flippedDiagonally = tmxTile.flippedDiagonally;
				tile.flippedHorizontally = tmxTile.flippedHorizontally;
				tile.flippedVertically = tmxTile.flippedVertically;
				tile.id = tmxTile.gid;
				tile.tileSheetId = tmxTile.tileSetId;
				row.push_back(tile);
			}

			layer.push_back(row);
		}

		tileGridLayers.push_back(layer);
	}

	for (TMXTileSet tileSet : data.tileSets)
	{
		graphics.loadTileSheet(tileSet.source, tileSet.tileWidth, tileSet.tileHeight, tileSet.spacing);
		tileSheetPaths.push_back(tileSet.source);
	}

	mapWidth = data.mapWidth;
	mapHeight = data.mapHeight;
	tileWidth = data.tileWidth;
	tileHeight = data.tileHeight;
}

void TileMap::draw(Graphics& graphics)
{
	// Portion of the tilemap being drawn
	Rectangle view = graphics.getView();
	int widthPixels = mapWidth * tileWidth;
	int heightPixels = mapHeight * tileHeight;

	int scaledTileWidth = (int)(tileWidth * graphics.getScale());
	int scaledTileHeight = (int)(tileHeight * graphics.getScale());

	// first and last tiles to draw
	int firstTileCol = view.getX() / scaledTileWidth;
	int firstTileRow = view.getY() / scaledTileHeight;
	int lastTileCol = (view.getX2() / scaledTileWidth) + 1;
	int lastTileRow = (view.getY2() / scaledTileHeight) + 1;
	// offset that is subtracted from the axis when starting drawing
	int drawOffsetX = view.getX() % scaledTileWidth;
	int drawOffsetY = view.getY() % scaledTileHeight;

	for (std::vector<std::vector<Tile>> layer : tileGridLayers)
	{
		for (int rowNum = firstTileRow; rowNum < lastTileRow; rowNum++)
		{
			for (int colNum = firstTileCol; colNum < lastTileCol; colNum++)
			{
				Tile tile = layer.at(rowNum).at(colNum);
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle destRect((colNum - firstTileCol) * scaledTileWidth - drawOffsetX,
						(rowNum - firstTileRow) * scaledTileHeight - drawOffsetY,
						scaledTileWidth,
						scaledTileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					// scaled is false because it's already scaled here
					graphics.drawImage(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, false);
				}
			}
		}
		//rowNum = 0;
		//for (std::vector<Tile> row : layer)
		//{
		//	colNum = 0;
		//	for (Tile tile : row)
		//	{
		//		if (tile.id >= 0 && tile.tileSheetId >= 0)
		//		{
		//			Rectangle destRect((int)(colNum * tileWidth * graphics.getScale()),
		//				(int)(rowNum * tileHeight * graphics.getScale()),
		//				tileWidth,
		//				tileHeight);

		//			std::string filePath = tileSheetPaths.at(tile.tileSheetId);
		//			graphics.drawImage(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, true);
		//		}

		//		colNum++;
		//	}
		//	rowNum++;
		//}
	}
}