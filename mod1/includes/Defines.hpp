#ifndef DEFINES_HPP
# define DEFINES_HPP

enum	e_coor
{
	e_000,
	e_100,
	e_110,
	e_101,
	e_111,
	e_010,
	e_011,
	e_001
};

// xyz v
// 000 0
// 100 0
// 110 0
// 101 126089
// 111 126089
// 010 0
// 011 441822
// 001 441822

#define EPS 1e-6

#define	MAX_COORDINATE 1000000
#define	MIN_COORDINATE 0

// #define I_DISTANCE	0.02 // interparticle distance
#define I_DISTANCE	500.0 // interparticle distance
#define E_RADIUS	(2.1 * I_DISTANCE) // effective radius
#define E_RADIUS_SQ (E_RADIUS * E_RADIUS)
#define	DIAMETER	(E_RADIUS / 2.0)
#define	RADIUS		(DIAMETER / 2.0)
#define WEIGHT(distance) ((E_RADIUS/distance) - 1.0)
#define KINEMATIC_VISCOSITY 0.000001
// #define KINEMATIC_VISCOSITY 0.000001 * I_DISTANCE / 0.02
#define SPEED_OF_SOUND 22.0
#define DENSITY_OF_PARTICLES 1000
#define DISTANCE_LIMIT (I_DISTANCE * 0.9)
#define DISTANCE_LIMIT_SQ (DISTANCE_LIMIT * DISTANCE_LIMIT)
#define REPULSION_COEFFICIENT 1.2

#define NUM_OF_PARTICLES 600
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1
#define COURANT_NUM 0.1
#define	BUCKET_LENGTH  (E_RADIUS * (1.0 + COURANT_NUM))
#define	NUM_OF_BUCKETS
#define InvisibleSpace (2.0 * BUCKET_LENGTH)

// #define DELTA_TIME 0.0005
#define DELTA_TIME 0.1
#define ONE_SECOUD 1.0
#define SIMULATION_TIME 100

#define X 0
#define Y 1
#define Z 2

#define IS_SPECIFIED 1
#define TRUE 1
#define FALSE 0

#define OMMIT 10000

#define RADIAN M_PI / 12
#define SCALING 0.7
#define DEPTH_SCALING 0.01f
#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

#endif