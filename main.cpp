#include <iostream>
#include <SFML/Graphics.hpp>

#include <vector>

#define S 10
#define F 40

class Tile;
std::vector< std::vector<Tile> > tiles;

class Tile {
	public:
		void Tick(int x, int y) {
			int num =
				NeighborLastTick(x - 1, y - 1) +
				NeighborLastTick(x - 1, y - 0) +
				NeighborLastTick(x - 1, y + 1) +
				NeighborLastTick(x + 0, y - 1) +
				NeighborLastTick(x + 0, y + 1) +
				NeighborLastTick(x + 1, y - 1) +
				NeighborLastTick(x + 1, y + 0) +
				NeighborLastTick(x + 1, y + 1);

			if(!mLiving) {
				mLiving = (num == 3);
			} else{
				mLiving = (num >= 2 && num <= 3);
			}
		}

		int NeighborLastTick(int x, int y) {
			if(x < 0) x = F - 1;
			if(x >= F) x = 0;
			if(y < 0) y = F - 1;
			if(y >= F) y = 0;

			if(tiles[x][y].mLastTickLiving)
				return 1;
			return 0;
		}


		void Draw(sf::RenderTarget* t, int x, int y) {
			sf::Color c = sf::Color::Black;
			if(mLiving) c = sf::Color::White;
			sf::Shape s = sf::Shape::Rectangle(
					x*S,
					y*S,
					S, S,
					c);
			t->Draw(s);
			mLastTickLiving = mLiving;
		}

		void Toggle() {
			mLastTickLiving = !mLastTickLiving;
			mLiving = !mLiving;
		}

		bool mLastTickLiving;
		bool mLiving;
};


int main() {

	for(int i = 0; i < F; ++i) {
		tiles.push_back( std::vector<Tile>() );
		for(int j = 0; j < F; ++j) {
			tiles[i].push_back( Tile() );
		}
	}



	sf::RenderWindow app(sf::VideoMode(400, 400, 32), "Conway's game of life - 30 minutes jam");


	sf::Clock clock;
	float time_since = 0;
	bool running = false;
	float speed = 0.2;

	while(app.IsOpened()) {
		float frame_time = clock.GetElapsedTime();
		clock.Reset();

		if(running)
			time_since += frame_time;


		sf::Event e;
		while(app.GetEvent(e)) {
			if(e.Type == sf::Event::Closed)
				app.Close();
			if(e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)
				app.Close();
			if(e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Space)
				running = !running;

			if(e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::C) {
				for(int i = 0; i < F; ++i) {
					for(int j = 0; j < F; ++j) {
						tiles[i][j].mLiving = false;
					}
				}
			}
			
			if(e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Up && speed > 0.02)
				speed -= 0.02;
			
			if(e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Down && speed < 1)
				speed += 0.02;




		}

		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Left)) {
			int x = app.GetInput().GetMouseX() / S;
			int y = app.GetInput().GetMouseY() / S;
			tiles[x][y].mLiving = true;
		}
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Right)) {
			int x = app.GetInput().GetMouseX() / S;
			int y = app.GetInput().GetMouseY() / S;
			tiles[x][y].mLiving = false;
		}

		app.Clear();


		if(time_since > speed) {
			for(int i = 0; i < F; ++i) {
				for(int j = 0; j < F; ++j) {
					tiles[i][j].Tick(i, j);
				}
			}
			time_since = 0;
		}

		for(int i = 0; i < F; ++i) {
			for(int j = 0; j < F; ++j) {
				tiles[i][j].Draw(&app, i, j);
			}
		}

		for(int i = 0; i < F; ++i) {
			app.Draw(sf::Shape::Line(i*S, 0, i*S, 400, 1, sf::Color(100,100,100,100)));
			app.Draw(sf::Shape::Line(0, i*S, 400, i*S, 1, sf::Color(100,100,100,100)));
		}

		app.Display();

	}
}

