#include "../includes/TerrestrialSimulation.hpp"
#include "../includes/ReadMapData.hpp"
#include "../includes/Graphic.hpp"
#include "../includes/Print.hpp"
#include "../includes/DelaunayTriangulation.hpp"
#include "../includes/Utils.hpp"
#include "../includes/MPS.hpp"

double	extend_map(const uint32_t mapSize)
{	
	const uint32_t	num = (uint32_t)((mapSize) / BUCKET_LENGTH);
	const double	diff = mapSize - num * BUCKET_LENGTH;

	return mapSize + BUCKET_LENGTH - diff + 4 * BUCKET_LENGTH;
}

TS::TS()
{

}

TS::~TS()
{

}

bool	TS::_ReadMapData(const std::string &fileName)
{
	RMD	rmd(fileName);

	if (rmd.ReadStart() == false)
	{
		return false;
	}
	this->_specificPoints = rmd.GetSpecificPoints();
	rmd.GetMapSize(this->_mapSize);
	rmd.GetMaxMinHeight(this->_maxHeight, this->_minHeight);

	return	true;
}

void	TS::_DrawSimulation(const int argc, const char** argv, const t_data	&data)
{
	Graphic g = Graphic(argc, argv, SCREEN_SIZE_X, SCREEN_SIZE_Y, data);
	g.GraphicLoop();
}

void	set_data(const std::deque<Triangle> &ts,
								 t_data &data, 
								 const uint32_t mapSize[3],
								 const int64_t maxHeight,
								 const int64_t minHeight)
{
	data.mapSize.x = extend_map(mapSize[X]);
	data.mapSize.y = extend_map(mapSize[Y]);
	data.mapSize.z = extend_map(mapSize[Z]);
	data.halfMapSize.x = data.mapSize.x / 2.0;
	data.halfMapSize.y = data.mapSize.y / 2.0;
	data.halfMapSize.z = data.mapSize.z / 2.0;
	data.ts = ts;
	for (size_t	i = 0; i < data.ts.size(); ++i)
	{
		data.ts[i].a.x += 2.0 * BUCKET_LENGTH;
		data.ts[i].a.y += 2.0 * BUCKET_LENGTH;
		data.ts[i].b.x += 2.0 * BUCKET_LENGTH;
		data.ts[i].b.y += 2.0 * BUCKET_LENGTH;
		data.ts[i].c.x += 2.0 * BUCKET_LENGTH;
		data.ts[i].c.y += 2.0 * BUCKET_LENGTH;
		data.ts[i].circumcircle.center.x += 2.0 * BUCKET_LENGTH;
		data.ts[i].circumcircle.center.y += 2.0 * BUCKET_LENGTH;
	}
	add_bottom(data.ts, Vec(mapSize[X],mapSize[Y],mapSize[Z]),
					data.mapSize,
					2.0 * BUCKET_LENGTH);
	add_cube(data.ts, data.mapSize);

	data.maxHeight = maxHeight;
	data.minHeight = minHeight;
	data.midHeight = (data.maxHeight + data.minHeight) / 2.0;
}

bool	TS::SimulationStart(const int argc, const char** argv, 
							const std::string &fileName)
{
	if (this->_ReadMapData(fileName) == false)
	{
		return false;
	}

	DT	dT(this->_specificPoints, this->_mapSize,
		   this->_maxHeight, this->_minHeight);
	std::deque<Triangle>	ts = dT.Calculation();
	t_data	data;

	set_data(ts, data, this->_mapSize,
					this->_maxHeight, this->_minHeight);

	MPS mps(data.mapSize, data.ts);
	mps.Simulation(data.logs);

	this->_DrawSimulation(argc, argv, data);

	return true;
}

void	TS::PrintSpecificPoints()
{
	for (size_t i = 0; i < this->_specificPoints.size(); ++i)
	{
		Print::OutWords(this->_specificPoints[i]);
	}
}
