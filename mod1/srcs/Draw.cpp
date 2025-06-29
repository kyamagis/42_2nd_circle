#include "../includes/Graphic.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Print.hpp"
#include "../includes/Particle.hpp"

static void	draw_vertex(const t_data &data, const Vec &vertex)
{
	const Vec		rotatedPos = vertex.Rotate(data.q);
	const double	coordinateX = rotatedPos.x / data.mapSize.x;
	const double	coordinateY =  - 1.0 * (rotatedPos.y / data.mapSize.y);
	const double	coordinateZ = rotatedPos.z / data.midHeight;
	const double	mousePosX = double(data.mouseX) / double(data.windowSizeX) - 0.5;
	const double	mousePosY = -(double(data.mouseY) / double(data.windowSizeY) - 0.5);

	glVertex3f((coordinateX - mousePosX) * data.scaling, 
			   (coordinateY - mousePosY) * data.scaling, 
			   -coordinateZ * DEPTH_SCALING * data.scaling);
}

static Vec	move_vec_to_map_center(const Vec &vec, const Vec &halfMapSize, const double midHeight)
{
	return	Vec(vec.x - halfMapSize.x,
				vec.y - halfMapSize.y, 
				vec.z - midHeight / 2.0);
}

static Vec	x_move_vec_to_map_center(const Vec &vec, const t_data &data)
{
	return	move_vec_to_map_center(vec, data.halfMapSize, data.midHeight);
}

static void	line_gradation(const int64_t	maxHeight,
						const int64_t	minHeight,
						const double	midHeight, 
						const int64_t height)
{
	double	ratio;

	if (height <= midHeight)
	{
		ratio = (height - minHeight) / (midHeight - minHeight);
		glColor3f(0.0f, ratio, 1.0f - ratio);
	}
	else if (height <= maxHeight)
	{
		ratio = (height - midHeight) / (maxHeight - midHeight);
		glColor3f(ratio, 1.0f - ratio, 0.0f);
	}
}

///////////////////////////////////////////////////////////////////////////////////////

static void	draw_triangle_frame(const t_data &data, const Triangle &triangle)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(triangle.b, data));
	draw_vertex(data, x_move_vec_to_map_center(triangle.a, data));
	draw_vertex(data, x_move_vec_to_map_center(triangle.c, data));
	glEnd();
}

void	draw_normal_vector(const t_data &data, const Triangle &triangle)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(triangle.g, data));
	draw_vertex(data, x_move_vec_to_map_center(triangle.g + triangle.n * 10, data));
	glEnd();
}

void	draw_triangle(const t_data &data, const Triangle &triangle)
{
	if (!triangle.visibleFlg)
	{
		draw_triangle_frame(data, triangle);
		return ;
	}
	if (data.lineFlg || !triangle.visibleFlg)
	{
		glBegin(GL_LINE_LOOP);
	}
	else
	{
		glShadeModel(GL_SMOOTH);
	}

	glBegin(GL_TRIANGLES);

	line_gradation(data.maxHeight, data.minHeight, data.midHeight, triangle.a.z);
	draw_vertex(data, x_move_vec_to_map_center(triangle.a, data));
	line_gradation(data.maxHeight, data.minHeight, data.midHeight, triangle.b.z);
	draw_vertex(data, x_move_vec_to_map_center(triangle.b, data));
	line_gradation(data.maxHeight, data.minHeight, data.midHeight, triangle.c.z);
	draw_vertex(data, x_move_vec_to_map_center(triangle.c, data));

	glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////

static void	draw_shortest_vec(const t_data &data, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position, data));
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position + bucket.shortestVec, data));
	glEnd();
}

static void	draw_normal_vec(const t_data &data, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position, data));
	glColor3f(0.0f, 0.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position + bucket.n * 1000, data));
	glEnd();
}

static void	draw_interpolated_normal_vec(const t_data &data, const t_bucket bucket)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position, data));
	glColor3f(1.0f, 0.0f, 0.0f);
	draw_vertex(data, x_move_vec_to_map_center(bucket.position + bucket.nInterpolation * 1000, data));
	glEnd();
}

static void	draw_bucket_pos(const t_data &data, const t_bucket bucket)
{
	const double	midDist = BUCKET_LENGTH / 2.0;

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	if (abs(bucket.distFromWall) < midDist)
	{
		glColor3f(0, abs(bucket.distFromWall) / midDist, 1 - abs(bucket.distFromWall) / midDist);
	}
	else
	{
		glColor3f(abs(bucket.distFromWall) / midDist, 1 - abs(bucket.distFromWall) / midDist, 0);
	}
	draw_vertex(data, x_move_vec_to_map_center(bucket.position, data));
	glEnd();
}

void	draw_dis_from_wall_SQ(const t_data &data, const BC &bc)
{	
	for (size_t	i = 0; i < bc.numOfBuckets; ++i)
	{
		if (abs(bc.buckets[i].distFromWall) <= bc.bc_bucketLength + EPS)
		{
			Print::OutWords(bc.buckets[i].bucketX, bc.buckets[i].bucketY, bc.buckets[i].bucketZ, bc.buckets[i].shortestVec);
			draw_bucket_pos(data, bc.buckets[i]);
			draw_shortest_vec(data, bc.buckets[i]);
			draw_normal_vec(data, bc.buckets[i]);
			draw_interpolated_normal_vec(data, bc.buckets[i]);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

static void	draw_circle(const t_data &data, 
						const Circle &circle,
						const size_t num_segments, Vec &c)
{
	double	theta;
	double	x;
	double	y;

	for (size_t i = 0; i < num_segments; ++i)
	{
		theta = 2.0 * M_PI * double(i) / double(num_segments);
		x = circle.r * cos(theta);
		y = circle.r * sin(theta);
		c.x = circle.center.x + x;
		c.y = circle.center.y + y;
		draw_vertex(data, x_move_vec_to_map_center(c, data));
	}
}

void	draw_circle_2d(const t_data &data, 
							const Circle &circle,
							const size_t num_segments) 
{
	Vec	c;

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);

	draw_circle(data, circle, num_segments, c);

	glEnd();
}

void	draw_circle_3d(const t_data &data, 
							const Circle &circle,
							const size_t num_segments) 
{
	double theta;
	double phi;
	Vec	north;
	Vec	south;
	const size_t	halfSegments = num_segments / 2 + 1;

	glBegin(GL_LINE_STRIP);
	glColor4f(circle.color.x, circle.color.y, circle.color.z, 0.6f);

	for (size_t i = 0; i < num_segments; ++i)
	{
		theta = i * M_PI / num_segments;
		for (size_t j = 0; j < halfSegments; ++j)
		{
			phi = j * 2.0f * M_PI / num_segments;

			north = circle.center;
			south = circle.center;

			north.x += circle.r * sin(theta) * cos(phi);
			north.y += circle.r * cos(theta);
			north.z += circle.r * sin(theta) * sin(phi);

			south.x += circle.r * sin(theta) * cos(-phi);
			south.y += circle.r * cos(theta);
			south.z += circle.r * sin(theta) * sin(-phi);

			draw_vertex(data, x_move_vec_to_map_center(north, data));
			draw_vertex(data, x_move_vec_to_map_center(south, data));
		}
	}
	glEnd();
}

void	draw_point_3d(const t_data &data,  const Particle &p)
{
	draw_vertex(data, x_move_vec_to_map_center(p.center, data));
}

///////////////////////////////////////////////////////////////////////////

void	draw_particle(const t_data &data, const Particle &p)
{
	draw_circle_3d(data, p, 10);
}

void	draw_point(const t_data &data, const Particle &p)
{
	draw_point_3d(data, p);
}

// void	draw_particles(const t_data &data, const std::deque<Particle> &ps, Vec &halfMapSize, const double midHeight)
// {
// 	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
// 	{
// 		draw_particle(data, ps[i], data.halfMapSize, data.midHeight);
// 		Print::OutWords(i, ps[i]);
// 	}
// }

////////////////////////////////////////////////////////////////////////////////


void	draw_particles(const t_data &data)
{
	if (SIMULATION_TIME < data.elapsedTime + 1)
	{
		Print::Err("DrawParticles: elapsedTime");
		return;
	}
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		draw_particle(data, data.logs[data.elapsedTime].ps[i]);
	}
}

void	draw_points(const t_data &data)
{
	if (SIMULATION_TIME < data.elapsedTime + 1)
	{
		Print::Err("DrawParticles: elapsedTime");
		return;
	}
	glEnable(GL_POINT_SMOOTH);
	glPointSize(20.0f);
    glBegin(GL_POINTS);
	glColor4f(0.0f, 1.0f, 1.0f, 0.4f);
	for (size_t	i = 0; i < NUM_OF_PARTICLES; ++i)
	{
		draw_point(data, data.logs[data.elapsedTime].ps[i]);
	}
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}
