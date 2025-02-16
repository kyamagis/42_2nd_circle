// Inverse Distance Weighting

#include <iostream>
#include <string.h>

#include "../includes/TerrestrialSimulation.hpp"

#define	POINTS_NUM 20
#define DIMENSION 3

void	SearchNeighborPoints(const int32_t ***threeDCoordinates,
								  const	uint32_t mapSize[2],
								  int32_t **&neighborPoints,
								  uint32_t x,
								  uint32_t y,
								  double  &sumOfWeightings)
{
	uint32_t	distance = 1;
	uint32_t	countOfPoints = 0;

	for (uint32_t	countOfPoints = 0; countOfPoints < POINTS_NUM ; ++distance)
	{
		for (int32_t dx = 0; dx * dx <= distance; ++dx)
		{
			for (int32_t dy = 0; dy * dy <= distance; ++dy)
			{
				if (distance == dx * dx + dy * dy)
				{
					if ()
					{

					}
				}
			}
		}
	}
}

void	IDW(int32_t ***&threeDCoordinates, const uint32_t mapSize[2])
{
	int32_t	neighborPoints[POINTS_NUM][DIMENSION];


	for (uint32_t x = 1; x < mapSize[X] - 1; ++x)
	{
		for (uint32_t y = 1; y < mapSize[Y] - 1; ++y)
		{
			if (threeDCoordinates[x][y][IS_SPECIFIED] == TRUE)
			{
				continue;
			}
			

		}
	}
}

