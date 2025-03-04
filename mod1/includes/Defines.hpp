#ifndef DEFINES_HPP
# define DEFINES_HPP

#define	MAX_COORDINATE 1000000
#define	MIN_COORDINATE 0

// #define I_DISTANCE	0.02 // interparticle distance
// #define E_RADIUS	(2.1 * I_DISTANCE) // effective radius
#define I_DISTANCE	10 // interparticle distance
#define E_RADIUS	(2.1 * I_DISTANCE) // effective radius
#define E_RADIUS_SQ (E_RADIUS * E_RADIUS)
#define	DIAMETER	(E_RADIUS / 2.0)
#define	RADIUS		(DIAMETER / 2.0)
#define WEIGHT(distance) ((E_RADIUS/distance) - 1.0)
#define KINEMATIC_VISCOSITY 0.000001
#define SPEED_OF_SOUND 22.0
#define DENSITY_OF_PARTICLES 1000
#define DISTANCE_LIMIT (I_DISTANCE * 0.9)

#define NUM_OF_PARTICLES 100
#define	D 3 // dimension number
#define	GRADIENT  0
#define LAPLACIAN 1
#define	BUCKET_LENGTH  E_RADIUS
#define	NUM_OF_BUCKETS
#define InvisibleSpace (2.0 * BUCKET_LENGTH)

#define X 0
#define Y 1
#define Z 2

#define IS_SPECIFIED 1
#define TRUE 1
#define FALSE 0

#define OMMIT 10000


#define SCALING 0.7
#define DEPTH_SCALING 0.01f
#define SCREEN_SIZE_X 1000
#define SCREEN_SIZE_Y 1000

#endif