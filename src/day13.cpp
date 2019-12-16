#include "days.h"
#include "intcode.h"
#include <map>
#include <tuple>

using std::vector;
using std::string;
using std::pair;
using std::tuple;

const int width = 256;
const int height = 256;

enum Tile
{
	Empty,
	Wall,
	Block,
	Horizontal,
	Ball
};


class Game
{
private:

	class Output : public BlockedOutputDevice<int64_t, 3>
	{
	private:
		Game& game;
	public:

		Output(Game& game) : game(game)
		{ }

		virtual void write(const Block& block) override
		{
			auto[x, y] = pair{ (int)block[0], (int)block[1] };

			if (x == -1 && y == 0)
			{
				game.score = block[2];
				return;
			}

			auto tile = (Tile)block[2];

			if (tile == Ball)
				game.ball = { x,y };
			
			if (tile == Horizontal)
				game.paddle = { x,y };

			game.tiles[x][y] = tile;
		}

		using BlockedOutputDevice::write;

		virtual void setEOF() override
		{
			// Computer be done
		}
	};

	class Input : public InputDevice<int64_t>
	{
		Game& game;
	public:
		Input(Game& game) : game(game)
		{ }
		std::optional<int64_t> read() override
		{
			if (game.ball.x > game.paddle.x)
				return 1;
			else if (game.ball.x < game.paddle.x)
				return -1;
			else
				return 0;
		}
	};

	typedef IO<Input, Output> IO;

	vector<vector<Tile>> tiles;
	IO io;
	Intcode<IO&> computer;
	int score = 0;
	util::Coord ball;
	util::Coord paddle;
public:

	Game(const string& program, bool insertCoin):
		tiles(width, vector<Tile>(height, Empty)),
		io{ Input(*this), Output(*this)},
		computer(program, io),
		ball {0,0},
		paddle {0,0}
	{
		if (insertCoin)
			computer.getMemory()[0] = 2;
	}

	void run()
	{
		computer.run();
	}
	
	int getScore()
	{
		return score;
	}

	int numBlocks()
	{
		int sum = 0;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (tiles[x][y] == Block) sum++;
			}
		}
		return sum;
	}

};


day(13) = solve(const auto & input)
{

	int64_t part1 = 0;
	int64_t part2 = 0;

	{
		Game game(input[0], false);
		game.run();

		part1 = game.numBlocks();
	}


	{
		Game game(input[0], true);

		game.run();

		part2 = game.getScore();

	}



	return { part1,part2 };
	
};