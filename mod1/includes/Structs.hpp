#ifndef STRUCTS_HPP
# define STRUCTS_HPP

#include "Defines.hpp"
#include "Particle.hpp"
#include "Triangle.hpp"

typedef struct s_log
{
	size_t		time;
	std::deque<Particle>	ps;
} t_log;

typedef struct s_data
{
	Vec						mapSize;
	Vec						halfMapSize;
	std::deque<Triangle>	ts;
	t_log					logs[SIMULATION_TIME];

	int64_t	maxHeight;
	int64_t	minHeight;
	double	midHeight;
	Vec		rad;
	Quaternion q;
	double	scaling;
	size_t	i;
	size_t	count;
	Vec		rotatedVertex;
	bool	circleFlg;
	bool	lineFlg;
	bool	visibleBucketsFlg;
	bool	simulationFlg;
	int		gWindowID;

	unsigned char	key;
	int				mouseX;
	int				mouseY;
	int				windowSizeX;
	int				windowSizeY;
	size_t			elapsedTime;

} t_data;

#endif