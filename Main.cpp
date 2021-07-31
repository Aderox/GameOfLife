#include <SDL.h>
#include <iostream>
#include <thread> 
#include <string>
#include <sstream>
#include <vector>

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")

const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 720;
const int OFFSET_MENU = 0;
const int SQUARE_SIZE_PX = 20;
const int NB_SQUARE_TOTAL = (int)WIN_WIDTH / SQUARE_SIZE_PX * (int)WIN_HEIGHT / SQUARE_SIZE_PX - ((int)OFFSET_MENU/SQUARE_SIZE_PX *(int)WIN_WIDTH / SQUARE_SIZE_PX);
typedef std::vector<int> int_vector;
int_vector cellArray(NB_SQUARE_TOTAL);


SDL_Window* window = nullptr;
SDL_Surface* windowSurface = nullptr;
SDL_Surface* iconSurface = nullptr;
SDL_Renderer* renderer = nullptr;


//void drawTriangle(SDL_Renderer* renderer, int posX, int posY);
//void draw_random(int nr_points, SDL_Renderer* renderer);
int calculateCoordonate(int x, int y);
void changeState(bool isFirst, bool drawing, int x, int y);
void renderSquare(int position, int r, int g, int b);
void drawLines();
void tick();

int oldPos = -1;
bool draw = NULL;
int firstPos = NULL;
bool isRunning = false;

int main(int argc, char* args[]) {
	printf("Hi !\n");
	//printf("vector size: %d\n", cellArray.size());

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Video Init Error: " << SDL_GetError() << std::endl;
	}
	else {
		window = SDL_CreateWindow("Ah ouais carrement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (window == NULL) {
			std::cout << "Window creation error: " << SDL_GetError() << std::endl;
		}
		else {
			//Window Created
			windowSurface = SDL_GetWindowSurface(window);
			iconSurface = SDL_LoadBMP("src/pics/you.bmp");

			isRunning = true;
			if (iconSurface == NULL) {
				std::cout << "Image loading error: " << SDL_GetError() << std::endl;
			}
			else {
				//Image Created

				//SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_SetWindowIcon(window, iconSurface);
			}


		}
	}
	
	SDL_Event ev;
	printf("\nloop\n");
	int i = 0;

	bool isClicking = false;
	bool drawing = true;
	bool playing = false;

	int MouseX = NULL;
	int MouseY = NULL;
	std::string str = "iteraton: " + 12;

	int speed = 1;
	while (isRunning) {
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) {
				printf("au revoir...");
				isRunning = false;
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (ev.button.button == SDL_BUTTON_LEFT) {
					drawing = true;
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT) {
					drawing = false;
				}
				SDL_PumpEvents();
				SDL_GetMouseState(&MouseX, &MouseY);
				changeState(true, drawing, MouseX, MouseY);
				int pos = calculateCoordonate(MouseX, MouseY);
				printf("coordone clique: %d %d. Position: %d\n", MouseX, MouseY, pos);
				isClicking = true;
			}
			else if (ev.type == SDL_MOUSEBUTTONUP) {
				//printf("no more clicking\n");
				isClicking = false;
			}
			else if (ev.type == SDL_KEYDOWN) {
				switch (ev.key.keysym.sym) {
				case SDLK_p :
					if (playing) { playing = false; }
					else { playing = true; }
					break;
				case SDLK_SPACE:
					if (playing) { playing = false; }
					else { playing = true; }
					break;
				case SDLK_t:
					tick();
				case SDLK_r:
					printf("todo !\n");
					break;
				case SDLK_c:
					printf("clean !\n");
					for (int i = 0; i < cellArray.size(); ++i) {
						cellArray[i] = 0;
					}
					break;
				case SDLK_1:
					speed = 50;
					break;
				case SDLK_2:
					speed = 30;
					break;
				case SDLK_3:
					speed = 20;
					break;
				case SDLK_4:
					speed = 10;
					break;
				case SDLK_5:
					speed = 5;
					break;
				case SDLK_6:
					speed = 4;
					break;
				case SDLK_7:
					speed = 3;
					break;
				case SDLK_8:
					speed = 2;
					break;
				case SDLK_9:
					speed = 1;
					break;
				}
			}
			
		}
		
		std::ostringstream titleStream;
		if (playing) {
			titleStream << "P/Space: Pause  T: One Tick  C: Clear  R: Randomise  1-9: Speed (currently " << 50*20 - speed*20 << ')';
		}
		else {
			titleStream << "P/Space: Resume  T: One Tick  C: Clear  R: Randomise  1-9: Speed (currently " << 50*20 -speed*20 << ')';
		}
		std::string titleStr = titleStream.str();
		SDL_SetWindowTitle(window, titleStr.c_str());


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		SDL_RenderClear(renderer);

		//Fait briller:
		//draw_random(700, renderer);


		drawLines();

		if (isClicking) {
			SDL_PumpEvents();
			SDL_GetMouseState(&MouseX, &MouseY);
			changeState(false, drawing, MouseX, MouseY);
			//printf("coordone clique: %d %d\n", MouseX, MouseY);
		}
		
		for (int i = 0; i < cellArray.size(); ++i) {
			if (cellArray[i] == 1) {
				renderSquare(i, 0, 0, 0);
			}
		}

		//limite à 60FPS (je crois)
		SDL_Delay(1);
		if (playing) {
			SDL_Delay(20 * speed);
			tick();
		}
		
		SDL_RenderPresent(renderer);
		//SDL_UpdateWindowSurface(window);

	}
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_FreeSurface(iconSurface);
	iconSurface = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;
	windowSurface = nullptr;

	SDL_Quit();
	return 0;

}

/*void draw_random(int nr_points, SDL_Renderer* renderer) {
	for (int i = 0; i < nr_points; ++i) {
		SDL_SetRenderDrawColor(renderer, randomColor(), randomColor(), randomColor(), 255);
		SDL_RenderDrawPoint(renderer, randomNumber(0, 1280), randomNumber(0, 720));
	}
}

void drawTriangle(SDL_Renderer* renderer, int posX, int posY) {
	SDL_RenderDrawLine(renderer, posX + 20, posY, posX, posY + 40);
	SDL_RenderDrawLine(renderer, posX, posY + 40, posX + 40, posY + 40);
	SDL_RenderDrawLine(renderer, posX + 40, posY + 40, posX + 20, posY);
}*/

int calculateCoordonate(int x, int y) {
	int posX = (int)x / SQUARE_SIZE_PX;
	int posY = (int)y / SQUARE_SIZE_PX;
	int nbCarre = (posY) * (int)WIN_WIDTH/ SQUARE_SIZE_PX + posX ;

	//printf("calcule des coo: x:%d y:%d;     posX: %d posY:%d;      Carre numero %d\n",x, y, posX, posY, nbCarre);

	return nbCarre;
}


void changeState(bool isFirst, bool drawing, int x, int y) {
	bool hasLeave = false;

	//only trigger by click event (once by click)
	if (isFirst) {
		//on prend l'inverse de la case cliqué en 1er
		firstPos = calculateCoordonate(x, y);
		if (firstPos >= 2112) {
			return;
		}
		if (drawing) {
			cellArray[firstPos] = 1;
		}
		else {
			cellArray[firstPos] = 0;
		}
	}
	//ce n'est pas l'event (on a glissé la souris)
	else {
		int pos = calculateCoordonate(x, y);
		if (pos < 2112 || OFFSET_MENU == 0) {
			//on verifie que la position est différent que celle précédante
			if (oldPos != pos) {
				//oldPos existe pas encore ou on est sorite du carré. Donc on a hasLeave=true
				oldPos = pos;
				hasLeave = true;
			}
			else {
				//la position n'a pas changé on fait rien
				hasLeave = false;
			}
			if (hasLeave) {
				//on a changer de case, et on "dessine"
				if (drawing) {
					cellArray[pos] = 1;
				}
				//on a changé et on "efface"
				else {
					cellArray[pos] = 0;
				}
			}
		}
	}
}

void renderSquare(int position, int r, int g, int b) {
	//Colorie un certain rectangle (determiné par sa position dans la liste) dans une couleur rgb
	int x, y;
	x = (int)position % ((int)WIN_WIDTH/SQUARE_SIZE_PX) * SQUARE_SIZE_PX;
	y = (int)position / ((int)WIN_WIDTH / SQUARE_SIZE_PX) * SQUARE_SIZE_PX;
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_Rect rect{ x, y, SQUARE_SIZE_PX, SQUARE_SIZE_PX };
	SDL_RenderFillRect(renderer, &rect);
}

void drawLines() {
	//Dessine le quadrillage
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < WIN_WIDTH; i += SQUARE_SIZE_PX) {
		SDL_RenderDrawLine(renderer, i, 0, i, WIN_HEIGHT - OFFSET_MENU);
	}
	for (int i = 0; i <= WIN_HEIGHT - OFFSET_MENU; i += SQUARE_SIZE_PX) {
		SDL_RenderDrawLine(renderer, 0, i, WIN_WIDTH, i);
	}
}


const int nbCote = (int)WIN_WIDTH / SQUARE_SIZE_PX;
const int nbLargeur = (int)WIN_HEIGHT / SQUARE_SIZE_PX;

int checkN(int i) {
	if (cellArray[i - nbCote] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int checkS(int i) {
	if (cellArray[i + (nbCote)] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int checkE(int i) {
	if (cellArray[i + 1] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int checkW(int i) {
	if (cellArray[i - 1] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int checkNW(int i) {
	if (cellArray[i - (nbCote + 1)] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int checkNE(int i) {
	if (cellArray[(i - nbCote) + 1] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int checkSW(int i) {
	if (cellArray[i + (nbCote - 1)] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int checkSE(int i) {
	if (cellArray[i + (nbCote + 1)] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int checkAll(int i) {
	int xu = 0;
	xu += checkN(i);
	xu += checkS(i);
	xu += checkE(i);
	xu += checkW(i);
	xu += checkNE(i);
	xu += checkNW(i);
	xu += checkSE(i);
	xu += checkSW(i);
	return xu;
}

void tick() {
	typedef std::vector<int> int_vector;
	int_vector cellArrayVoisin(NB_SQUARE_TOTAL);
	

	for (int i = 0; i < cellArray.size(); i++) {
		int nbVoisin = 0;
			//soit le quadrillage: (on est en E)
			//NW N NE
			//W X E
			//SW S SE

			//verifie qu'on est pas au bord gauche: (int)i / nbCote-  1 > 0
			//verifie qu'on est pas au bord droit: (int)i / nbCote - 1 < nbCote
			//verifie qu'on est pas tout en haut: (int)i / nbLargeur - 1 > 0
			//verifie qu'on est pas tout en bas: (int)i / nbLargeur - 1 < 0

			//AGGGGGB
			//C.....D
 			//C.....D
			//EHHHHHF

			if (i == 0) { //A
				nbVoisin += checkE(i);
				nbVoisin += checkS(i);
				nbVoisin += checkSE(i);
			}
			else if(i == nbCote-1){ //B
				nbVoisin += checkW(i);
				nbVoisin += checkS(i);
				nbVoisin += checkSW(i);
			}
			else if (i == cellArray.size() - nbCote) { //E
				nbVoisin += checkN(i);
				nbVoisin += checkE(i);
				nbVoisin += checkNE(i);
			}
			else if (i == cellArray.size() - 1) { //F
				nbVoisin += checkW(i);
				nbVoisin += checkN(i);
				nbVoisin += checkNW(i);
			}
			else if(i % nbCote == 0){ //C
				nbVoisin += checkN(i);
				nbVoisin += checkS(i);
				nbVoisin += checkE(i);
				nbVoisin += checkNE(i);
				nbVoisin += checkSE(i);
			}
			else if (i % nbCote == nbCote-1) {//D
				nbVoisin += checkN(i);
				nbVoisin += checkS(i);
				nbVoisin += checkW(i);
				nbVoisin += checkNW(i);
				nbVoisin += checkSW(i);
			}
			else if (i > 0 && i<nbCote) {//G
				nbVoisin += checkE(i);
				nbVoisin += checkW(i);
				nbVoisin += checkS(i);
				nbVoisin += checkSW(i);
				nbVoisin += checkSE(i);
			}
			else if (i > cellArray.size() - nbCote && i < cellArray.size() -1) {//H
				nbVoisin += checkE(i);
				nbVoisin += checkW(i);
				nbVoisin += checkN(i);
				nbVoisin += checkNW(i);
				nbVoisin += checkNE(i);
			}
			else {
				nbVoisin = checkAll(i);
			}

			//nbVoisin += checkN(i);
			// wikipedia:
			//une cellule morte possédant exactement trois voisines vivantes devient vivante (elle naît) ;
			//une cellule vivante possédant deux ou trois voisines vivantes le reste, sinon elle meurt.
			//----

			cellArrayVoisin[i] = nbVoisin;
			//if (nbVoisin != 0) {
				//printf("Nombre de voisin de %d:%d\n", i, nbVoisin);
			//}
			
			
			
			
	}
	
	
	for (int j = 0; j < cellArrayVoisin.size(); ++j) {
		if (cellArrayVoisin[j] == 0 && cellArray[j] == 0) {
			continue;
		}
		else if (cellArrayVoisin[j] == 3 && cellArray[j] == 0) {
			cellArray[j] = 1;
		}
		else if (cellArray[j] == 1) {
			if (cellArrayVoisin[j] == 2 || cellArrayVoisin[j] == 3) {
				continue;
			}
			else {
				cellArray[j] = 0;
			}
		}
	}
}
