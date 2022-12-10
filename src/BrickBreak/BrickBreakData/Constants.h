#pragma once

#include "NightOwl/Math/Math.h"
#include <vector>

namespace BrickBreak
{
	static constexpr float SCREEN_PIXEL_WIDTH = 338.0f;
	static constexpr float SCREEN_PIXEL_HEIGHT = 253.5f;

	static constexpr float SCREEN_HALF_PIXEL_WIDTH = SCREEN_PIXEL_WIDTH * 0.5;
	static constexpr float SCREEN_HALF_PIXEL_HEIGHT = SCREEN_PIXEL_HEIGHT * 0.5;

	static constexpr float TEXTURE_HEIGHT = 119.0f;
	static constexpr float TEXTURE_WIDTH  = 113.0f;

	static constexpr float BRICK_WIDTH  = 30.0f;
	static constexpr float BRICK_HEIGHT = 15.0f;

	static constexpr float BRICK_HALF_WIDTH = BRICK_WIDTH * 0.5f;
	static constexpr float BRICK_HALF_HEIGHT = BRICK_HEIGHT * 0.5f;

	static constexpr float BALL_WIDTH  = 9.0f;
	static constexpr float BALL_HEIGHT = 9.0f;

	static constexpr float BALL_HALF_WIDTH = BALL_WIDTH * 0.5f;
	static constexpr float BALL_HALF_HEIGHT = BALL_HEIGHT * 0.5f;

	static constexpr float PADDLE_WIDTH  = 72.0f;
	static constexpr float PADDLE_HEIGHT =  14.0f;

	static constexpr float PADDLE_HALF_WIDTH  = PADDLE_WIDTH  * 0.5f;
	static constexpr float PADDLE_HALF_HEIGHT = PADDLE_HEIGHT * 0.5f;

	static constexpr float WALL_WIDTH = 19.0f;
	static constexpr float WALL_HEIGHT = 119.0f;

	static constexpr float WALL_HALF_WIDTH = WALL_WIDTH * 0.5f;
	static constexpr float WALL_HALF_HEIGHT = WALL_HEIGHT * 0.5f;

	static constexpr float BALL_X_TEXEL_BEG = 0.0f;
	static constexpr float BALL_X_TEXEL_END = BALL_WIDTH / TEXTURE_WIDTH;
	static constexpr float BALL_Y_TEXEL_BEG = 96.01f / TEXTURE_HEIGHT;
	static constexpr float BALL_Y_TEXEL_END = BALL_Y_TEXEL_BEG + (BALL_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float PADDLE_X_TEXEL_BEG = 0.0f;
	static constexpr float PADDLE_X_TEXEL_END = PADDLE_WIDTH / TEXTURE_WIDTH;
	static constexpr float PADDLE_Y_TEXEL_BEG = 105.01f / TEXTURE_HEIGHT;
	static constexpr float PADDLE_Y_TEXEL_END = PADDLE_Y_TEXEL_BEG + (PADDLE_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float RED_BRICK_X_TEXEL_BEG = 0.0f;
	static constexpr float RED_BRICK_X_TEXEL_END = BRICK_WIDTH / TEXTURE_WIDTH;
	static constexpr float RED_BRICK_Y_TEXEL_BEG = 81.0f / TEXTURE_HEIGHT;
	static constexpr float RED_BRICK_Y_TEXEL_END = RED_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float BLUE_BRICK_X_TEXEL_BEG = 0.0f;
	static constexpr float BLUE_BRICK_X_TEXEL_END = BRICK_WIDTH / TEXTURE_WIDTH;
	static constexpr float BLUE_BRICK_Y_TEXEL_BEG = 65.0f / TEXTURE_HEIGHT;
	static constexpr float BLUE_BRICK_Y_TEXEL_END = BLUE_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float GREEN_BRICK_X_TEXEL_BEG = 0.0f;
	static constexpr float GREEN_BRICK_X_TEXEL_END = BRICK_WIDTH / TEXTURE_WIDTH;
	static constexpr float GREEN_BRICK_Y_TEXEL_BEG = 49.0f / TEXTURE_HEIGHT;
	static constexpr float GREEN_BRICK_Y_TEXEL_END = GREEN_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float YELLOW_BRICK_X_TEXEL_BEG = 31.0f / TEXTURE_WIDTH;
	static constexpr float YELLOW_BRICK_X_TEXEL_END = YELLOW_BRICK_X_TEXEL_BEG + (BRICK_WIDTH / TEXTURE_WIDTH);
	static constexpr float YELLOW_BRICK_Y_TEXEL_BEG = 81.0f / TEXTURE_HEIGHT;
	static constexpr float YELLOW_BRICK_Y_TEXEL_END = YELLOW_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float PURPLE_BRICK_X_TEXEL_BEG = 31.0f / TEXTURE_WIDTH;
	static constexpr float PURPLE_BRICK_X_TEXEL_END = PURPLE_BRICK_X_TEXEL_BEG + (BRICK_WIDTH / TEXTURE_WIDTH);
	static constexpr float PURPLE_BRICK_Y_TEXEL_BEG = 65.0f / TEXTURE_HEIGHT;
	static constexpr float PURPLE_BRICK_Y_TEXEL_END = PURPLE_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float ORANGE_BRICK_X_TEXEL_BEG = 31.0f / TEXTURE_WIDTH;
	static constexpr float ORANGE_BRICK_X_TEXEL_END = ORANGE_BRICK_X_TEXEL_BEG + (BRICK_WIDTH / TEXTURE_WIDTH);
	static constexpr float ORANGE_BRICK_Y_TEXEL_BEG = 49.0f / TEXTURE_HEIGHT;
	static constexpr float ORANGE_BRICK_Y_TEXEL_END = ORANGE_BRICK_Y_TEXEL_BEG + (BRICK_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float LEFT_WALL_X_TEXEL_BEG = 94.0f / TEXTURE_WIDTH;
	static constexpr float LEFT_WALL_X_TEXEL_END = LEFT_WALL_X_TEXEL_BEG + (WALL_WIDTH / TEXTURE_WIDTH);
	static constexpr float LEFT_WALL_Y_TEXEL_BEG = 0.0f;
	static constexpr float LEFT_WALL_Y_TEXEL_END = LEFT_WALL_Y_TEXEL_BEG + (WALL_HEIGHT / TEXTURE_HEIGHT);

	static constexpr float RIGHT_WALL_X_TEXEL_BEG = 75.0f / TEXTURE_WIDTH;
	static constexpr float RIGHT_WALL_X_TEXEL_END = RIGHT_WALL_X_TEXEL_BEG + (WALL_WIDTH / TEXTURE_WIDTH);
	static constexpr float RIGHT_WALL_Y_TEXEL_BEG = 0.0f;
	static constexpr float RIGHT_WALL_Y_TEXEL_END = RIGHT_WALL_Y_TEXEL_BEG + (WALL_HEIGHT / TEXTURE_HEIGHT);

	static const std::vector<NightOwl::Math::Vec3F> BRICK_VERTICES = 
	{
		{ -BRICK_HALF_WIDTH, -BRICK_HALF_HEIGHT, 0.0f },
		{ -BRICK_HALF_WIDTH,  BRICK_HALF_HEIGHT, 0.0f },
		{  BRICK_HALF_WIDTH, -BRICK_HALF_HEIGHT, 0.0f },
		{  BRICK_HALF_WIDTH,  BRICK_HALF_HEIGHT, 0.0f }
	};

	static const std::vector<NightOwl::Math::Vec3F> BALL_VERTICES =
	{
		{ -BALL_HALF_WIDTH, -BALL_HALF_HEIGHT, 0.0f },
		{ -BALL_HALF_WIDTH,  BALL_HALF_HEIGHT, 0.0f },
		{  BALL_HALF_WIDTH, -BALL_HALF_HEIGHT, 0.0f },
		{  BALL_HALF_WIDTH,  BALL_HALF_HEIGHT, 0.0f }
	};

	static const std::vector<NightOwl::Math::Vec3F> PADDLE_VERTICES =
	{
		{ -PADDLE_HALF_WIDTH, -PADDLE_HALF_HEIGHT, 0.0f },
		{ -PADDLE_HALF_WIDTH,  PADDLE_HALF_HEIGHT, 0.0f },
		{  PADDLE_HALF_WIDTH, -PADDLE_HALF_HEIGHT, 0.0f },
		{  PADDLE_HALF_WIDTH,  PADDLE_HALF_HEIGHT, 0.0f }
	};

	static const std::vector<NightOwl::Math::Vec3F> WALL_VERTICES =
	{
		{ -WALL_HALF_WIDTH, -WALL_HALF_HEIGHT, 0.0f },
		{ -WALL_HALF_WIDTH,  WALL_HALF_HEIGHT, 0.0f },
		{  WALL_HALF_WIDTH, -WALL_HALF_HEIGHT, 0.0f },
		{  WALL_HALF_WIDTH,  WALL_HALF_HEIGHT, 0.0f }
	};

	static const std::vector<NightOwl::Math::Vec3F> YOU_WIN_LOSE_VERTICES =
	{
		{ -SCREEN_HALF_PIXEL_WIDTH, -SCREEN_HALF_PIXEL_HEIGHT, 0.0f },
		{ -SCREEN_HALF_PIXEL_WIDTH,  SCREEN_HALF_PIXEL_HEIGHT, 0.0f },
		{  SCREEN_HALF_PIXEL_WIDTH, -SCREEN_HALF_PIXEL_HEIGHT, 0.0f },
		{  SCREEN_HALF_PIXEL_WIDTH,  SCREEN_HALF_PIXEL_HEIGHT, 0.0f }
	};
	static const std::vector<NightOwl::Math::Vec3UI> QUAD_TRIANGLES =
	{
		{ 0, 3, 1 },
		{ 0, 2, 3 }
	};

	static const std::vector<NightOwl::Math::Vec2F> BALL_UVS =
	{
		{ BALL_X_TEXEL_BEG, BALL_Y_TEXEL_BEG },
		{ BALL_X_TEXEL_BEG, BALL_Y_TEXEL_END },
		{ BALL_X_TEXEL_END, BALL_Y_TEXEL_BEG },
		{ BALL_X_TEXEL_END, BALL_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> PADDLE_UVS =
	{
		{ PADDLE_X_TEXEL_BEG, PADDLE_Y_TEXEL_BEG },
		{ PADDLE_X_TEXEL_BEG, PADDLE_Y_TEXEL_END },
		{ PADDLE_X_TEXEL_END, PADDLE_Y_TEXEL_BEG },
		{ PADDLE_X_TEXEL_END, PADDLE_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> RED_BRICK_UVS= 
	{
		{ RED_BRICK_X_TEXEL_BEG, RED_BRICK_Y_TEXEL_BEG },
		{ RED_BRICK_X_TEXEL_BEG, RED_BRICK_Y_TEXEL_END },
		{ RED_BRICK_X_TEXEL_END, RED_BRICK_Y_TEXEL_BEG },
		{ RED_BRICK_X_TEXEL_END, RED_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> BLUE_BRICK_UVS = 
	{
		{ BLUE_BRICK_X_TEXEL_BEG, BLUE_BRICK_Y_TEXEL_BEG },
		{ BLUE_BRICK_X_TEXEL_BEG, BLUE_BRICK_Y_TEXEL_END },
		{ BLUE_BRICK_X_TEXEL_END, BLUE_BRICK_Y_TEXEL_BEG },
		{ BLUE_BRICK_X_TEXEL_END, BLUE_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> GREEN_BRICK_UVS = 
	{
		{ GREEN_BRICK_X_TEXEL_BEG, GREEN_BRICK_Y_TEXEL_BEG },
		{ GREEN_BRICK_X_TEXEL_BEG, GREEN_BRICK_Y_TEXEL_END },
		{ GREEN_BRICK_X_TEXEL_END, GREEN_BRICK_Y_TEXEL_BEG },
		{ GREEN_BRICK_X_TEXEL_END, GREEN_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> YELLOW_BRICK_UVS = 
	{
		{ YELLOW_BRICK_X_TEXEL_BEG, YELLOW_BRICK_Y_TEXEL_BEG },
		{ YELLOW_BRICK_X_TEXEL_BEG, YELLOW_BRICK_Y_TEXEL_END },
		{ YELLOW_BRICK_X_TEXEL_END, YELLOW_BRICK_Y_TEXEL_BEG },
		{ YELLOW_BRICK_X_TEXEL_END, YELLOW_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> PURPLE_BRICK_UVS = 
	{
		{ PURPLE_BRICK_X_TEXEL_BEG, PURPLE_BRICK_Y_TEXEL_BEG },
		{ PURPLE_BRICK_X_TEXEL_BEG, PURPLE_BRICK_Y_TEXEL_END },
		{ PURPLE_BRICK_X_TEXEL_END, PURPLE_BRICK_Y_TEXEL_BEG },
		{ PURPLE_BRICK_X_TEXEL_END, PURPLE_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> ORANGE_BRICK_UVS = 
	{
		{ ORANGE_BRICK_X_TEXEL_BEG, ORANGE_BRICK_Y_TEXEL_BEG },
		{ ORANGE_BRICK_X_TEXEL_BEG, ORANGE_BRICK_Y_TEXEL_END },
		{ ORANGE_BRICK_X_TEXEL_END, ORANGE_BRICK_Y_TEXEL_BEG },
		{ ORANGE_BRICK_X_TEXEL_END, ORANGE_BRICK_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> LEFT_WALL_UVS =
	{
		{ LEFT_WALL_X_TEXEL_BEG, LEFT_WALL_Y_TEXEL_BEG },
		{ LEFT_WALL_X_TEXEL_BEG, LEFT_WALL_Y_TEXEL_END },
		{ LEFT_WALL_X_TEXEL_END, LEFT_WALL_Y_TEXEL_BEG },
		{ LEFT_WALL_X_TEXEL_END, LEFT_WALL_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> RIGHT_WALL_UVS =
	{
		{ RIGHT_WALL_X_TEXEL_BEG, RIGHT_WALL_Y_TEXEL_BEG },
		{ RIGHT_WALL_X_TEXEL_BEG, RIGHT_WALL_Y_TEXEL_END },
		{ RIGHT_WALL_X_TEXEL_END, RIGHT_WALL_Y_TEXEL_BEG },
		{ RIGHT_WALL_X_TEXEL_END, RIGHT_WALL_Y_TEXEL_END }
	};

	static const std::vector<NightOwl::Math::Vec2F> YOU_WIN_LOSE_UVS =
	{
		{ 0, 0 },
		{ 0, 1 },
		{ 1, 0 },
		{ 1, 1 }
	};
}
