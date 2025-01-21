#include <SFML/Graphics.hpp>
#include <time.h> 

using namespace sf;

const int M = 20;  // Высота поля
const int N = 10;  // Ширина поля

int field[M][N] = {0};  //  Само поле

int figures[7][4]=
{
	1,3,5,7, // I
	2,4,5,7, // S
	3,5,4,6, // Z
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
    int x, y;
} a[4],b[4];  // а - нужен для перемещения, b - для проверки границ поля

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
 
	return 1;
}

int main()
{	
    srand(time(0));

	RenderWindow window(VideoMode(320, 480), "The Tetris!");
 
    Texture texture, texture_background, texture_frame;
    texture.loadFromFile("/home/max/practice/Tetris/images/tiles.png");
	texture_background.loadFromFile("/home/max/practice/Tetris/images/background.png");
	texture_frame.loadFromFile("/home/max/practice/Tetris/images/frame.png");

	// Создание спрайта
	Sprite sprite(texture), sprite_background(texture_background), sprite_frame(texture_frame);
	
	// Вырезаем из спрайта отдельный квадратик размером 18х18 пикселей
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;
    bool beginGame = true;
    int n = rand() % 7; // Тип тетрамино

    float timer = 0, delay = 0.3;

    Clock clock;

    dx = 0;
    rotate = 0;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
	    clock.restart();
	    timer += time;

		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				window.close();

            if(event.type == Event::KeyPressed)

                if (event.key.code == Keyboard::Up) rotate = true;
                
                else if(event.key.code == Keyboard::Left) dx = -1;

                else if(event.key.code == Keyboard::Right) dx = 1; 
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;
        for(int i = 0; i < 4; i++){
            b[i] = a[i];
            a[i].x += dx;
        }  // Горизонтальное перемещение 

        if (!check()) {  // Проверка края поля
            for (int i = 0; i < 4; i++) 
            a[i] = b[i]; 
}

        if (rotate)
		{
			Point p = a[1]; // указываем центр вращения
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y; // y - y0
				int y = a[i].x - p.x; // x - x0
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
            if (!check()) { // проверка границ
                for (int i = 0; i < 4; i++)
                a[i] = b[i];
            }
		}

        if (timer > delay)
        {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;
				colorNum = 1 + rand() % 7;
				n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
				
			}
			timer = 0;
        }

        int k = M - 1; // есть ли линия для уничтожения
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

        if(beginGame) // Первое появление тетрамино?
        {
            beginGame = false;
            n = rand() % 7;
            for (int i = 0; i < 4; i++)
		    {
			    a[i].x = figures[n][i] % 2; 
			    a[i].y = figures[n][i] / 2;  
		    }
        }
        dx = 0, rotate = 0, delay = 0.3;

		// Установка цвета фона - белый
		window.clear(Color::White);
        window.draw(sprite_background);
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31); // смещение
				window.draw(sprite);
			}

		for (int i = 0; i < 4; i++)
		{
			// Разукрашиваем тетрамино
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			
			// Устанавливаем позицию каждого кусочка тетрамино
			sprite.setPosition(a[i].x * 18, a[i].y * 18);
			
			sprite.move(28, 31); // смещение
			
			// Отрисовка спрайта
			window.draw(sprite);
		}

        window.draw(sprite);
		// Отрисовка окна
		window.display();
	}
 
	return 0;
}