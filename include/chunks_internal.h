#ifndef CHUNKS_INTERNAL_H
#define CHUNKS_INTERNAL_H

#include "chunk.h"

extern Chunk* chunks;
extern Mesh* chunk_meshes;

extern Mesh chunk_debug_mesh;

static const Vector3 neighbour_dirs[6] = {
	{  1.f,  0.f,  0.f },
	{ -1.f,  0.f,  0.f },
	{  0.f,  1.f,  0.f },
	{  0.f, -1.f,  0.f },
	{  0.f,  0.f,  1.f },
	{  0.f,  0.f, -1.f }
};

// this makes the code below simpler
static const Vector3 quad_vertices[36] = {
	{  .5f,  .5f,  .5f }, // right
	{  .5f,  .5f, -.5f },
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f,  .5f },
	{  .5f,  .5f,  .5f },

	{ -.5f,  .5f,  .5f }, // left
	{ -.5f,  .5f, -.5f },
	{ -.5f, -.5f, -.5f },
	{ -.5f, -.5f, -.5f },
	{ -.5f, -.5f,  .5f },
	{ -.5f,  .5f,  .5f },

	{ -.5f, -.5f, -.5f }, // bottom
	{  .5f, -.5f, -.5f },
	{  .5f, -.5f,  .5f },
	{  .5f, -.5f,  .5f },
	{ -.5f, -.5f,  .5f },
	{ -.5f, -.5f, -.5f },

	{ -.5f,  .5f, -.5f }, // top
	{  .5f,  .5f, -.5f },
	{  .5f,  .5f,  .5f },
	{  .5f,  .5f,  .5f },
	{ -.5f,  .5f,  .5f },
	{ -.5f,  .5f, -.5f },

	{ -.5f, -.5f,  .5f }, // front
	{  .5f, -.5f,  .5f },
	{  .5f,  .5f,  .5f },
	{  .5f,  .5f,  .5f },
	{ -.5f,  .5f,  .5f },
	{ -.5f, -.5f,  .5f },

	{ -.5f, -.5f, -.5f }, // back
	{  .5f, -.5f, -.5f },
	{  .5f,  .5f, -.5f },
	{  .5f,  .5f, -.5f },
	{ -.5f,  .5f, -.5f },
	{ -.5f, -.5f, -.5f }
};

static const Vector2 quad_uvs[36] = {
	{ 1.f, 0.f }, // left
	{ 1.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 0.f },
	{ 1.f, 0.f },

	{ 1.f, 0.f }, // right
	{ 1.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 1.f },
	{ 0.f, 0.f },
	{ 1.f, 0.f },

	{ 0.f, 1.f }, // top
	{ 1.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 0.f },
	{ 0.f, 1.f },

	{ 0.f, 1.f }, // bottom
	{ 1.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 0.f },
	{ 0.f, 1.f },

	{ 0.f, 1.f }, // front
	{ 1.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 0.f },
	{ 0.f, 1.f },

	{ 0.f, 1.f }, // back
	{ 1.f, 1.f },
	{ 1.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 0.f },
	{ 0.f, 1.f },
};

#endif
