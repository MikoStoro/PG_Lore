#define _USE_MATH_DEFINE
#define COLORSNUM 4 //the number of colord used by finctions
#define PLAYER_HEALTH 5 //points
#define PI 3.14159265
#define RESPAWNTIME 5 //seconds
#define IFRAMES 1 //seconds
#define LEVELS 3 //number of levels
#define SCREEN_WIDTH	640 //pixels
#define SCREEN_HEIGHT	480 //pixels

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

enum colors {
	BLACK,
	GREEN,
	RED,
	BLUE,
};

enum axes {
	X,
	Y
};

enum dirs {
	N,
	E,
	S,
	W
};

enum attacks {
	NONE,
	ONEBULLET,
	CONE3,
	CONE5,
	CIRCLE
};

enum movements {
	STATIC,
	STRAIGHT,
	FOLLOW,
	BOUNCE
};

enum state {
	MENU,
	LEVEL,
	GAMEOVER
};

struct Program;
struct Game;
struct Npc;
struct Entity;
struct EntityListNode;

int spriteErrorHandler(SDL_Surface* sprite, char* name, Program* program);
void cleanupLevel(Program* program);
void menuLoop(Program* program);
void startLevel(Program* program);
Entity createEnemy(char* sprite, Program* program, int movement = STATIC, int attack1 = NULL, int attack2 = NULL, int cdTime = 1, int speed = 200, int health = NULL);
void selectLevel(int levelId, Program* program);

void pushEntityToList(EntityListNode* node, Entity* entity);
int countNodes(EntityListNode* node);
void removeNode(EntityListNode* node);
void clearList(EntityListNode* node);

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

int setupProgram(Program* program);
void setupProgramVariables(Program* program);
void setPlayerSprites(Program* program, char* n, char* e, char* w, char* s);
void setPlayerSprites(Program* program, char* n, char* e, char* w, char* s);
void setColors(Program* program);
int spriteErrorHandler(SDL_Surface* sprite, char* name, Program* program);
void newFrame(Program* program);
void renderFrame(Program* program);
void updateEntityPosition(Entity* entity, Program* program);
void updatePlayerPosition(Program* program);
void setEntityPosition(Entity* entity, int x, int y);
int loadEntitySprite(Entity* entity, char* path, Program* program);
int loadBackground(char* path, Program* program);
int setBulletSprite(char* path, Program* program);
int detectCollision(Entity* e1, Entity* e2);
void setEntityHitbox(Entity* entity, int x, int y);
Entity* spawnBullet(double ang, int x, int y, Program* program, int speed = 100);
Entity* spawnPlayerBullet(Program* program, int speed = 100);
Entity* getEnemy(Program* program);
double getPlayerAngle(Entity* entity, Program* program);
void setEntityVector(Entity* entity, double ang);
int selectAttack(Entity* entity);
void performAttack(Entity* entity, int attackCode, Program* program);
char getGrade(int score);
void performMovement(Entity* entity, int movementCode, Program* program);
void shoot(Program* program);
int dealDamage(Entity* entity, Entity* target);
void gameOverLoop(Program* program);
void cleanupLevel(Program* program);
void drawBackground(Program* program);
void drawBorder(Program* program);
void gameLoop(Program* program);
void startLevel(Program* program);
void levelSetUp(char* background, int pickupRespawn, Program* program);
void selectLevel(int levelId, Program* program);
void menuLoop(Program* program);


struct Entity {
	int speed = 1;
	float vector[2] = { 0,0 };
	int direction = N;
	float position[2] = { 0,0 };

	int killable = 1;
	int attack1 = NULL;
	int attack2 = NULL;
	int movement = STATIC;
	int maxhealth = PLAYER_HEALTH;
	int health = PLAYER_HEALTH;
	int damage = 0;
	int cdTime = 3;
	float cdTimer = 0;
	float iframeTimer = 999;
	int hitbox[2];

	SDL_Surface* sprite;
};

struct EntityListNode {
	EntityListNode* prev = NULL;
	Entity* entity = NULL;
	EntityListNode* next = NULL;
};

//obs≥uga lsity
void pushEntityToList(EntityListNode* node, Entity* entity) {
	if (node->entity == NULL) {
		node->entity = entity;
	}else if (node->next == NULL) {
		node->next = new EntityListNode;
		node->next->entity = entity;
		node->next->prev = node;
	}
	else {
		pushEntityToList(node->next, entity);
	}
}

int countNodes(EntityListNode* node) {
	int res = 0;
	while (node) {
		res += 1;
		node = node->next;
	}
	return res;
}

void removeNode(EntityListNode* node) {
	if (node->entity) {
		delete(node->entity);
		node->entity = NULL;
	}
	if (node->prev && node->next) {
		node->prev->next = node->next;
		node->next->prev = node->prev;
		delete node;
		node = NULL;
	}
	else if (node->next) {
		//do nothing
	}
	else if (node->prev) {
		node->prev->next = NULL;
		delete node;
		node = NULL;
	}
}

void clearList(EntityListNode* node) {
	if (node) {
		EntityListNode* next = node->next;
		if (node->entity) { delete(node->entity); node->entity = NULL; }
		if (node->prev) { node->prev = NULL; node->next = NULL; delete node; node = NULL; clearList(next); }
		else { node->next = NULL; node = NULL; clearList(next); }

	}
}


struct Game {
	Entity* player = new Entity;
	SDL_Surface* playerSprites[4];
	int levelId = NULL;
	int levelWidth = 2000;
	int levelheight = 2000;
	int pickupRespawn = 4;
	float pickupRespawnTimer = 0;
	int score = 0;
	int positiveMultiplier = 1;
	int negativeMultiplier = 1;

	//camera[0] = x, camera[1] = y
	float camera[2] = {float(levelWidth/2 - SCREEN_WIDTH/2), float(levelheight/2-SCREEN_HEIGHT/2)};

	Entity* enemyData;
	Entity* healthPickup;
	EntityListNode* bulletList = new EntityListNode;
	EntityListNode* playerBullets = new EntityListNode;
	EntityListNode* healthPickupList = new EntityListNode;
	SDL_Surface* bulletSprite = new SDL_Surface;
	SDL_Surface* background = new SDL_Surface;
};

struct Program {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps;
	int palette[COLORSNUM];
	char text[128];
	int state = MENU;

	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	Game* game = new Game;
};



// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

int setupProgram(Program* program) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe≥noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	program->rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &(program->window), &(program->renderer));
	if(program->rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(program->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(program->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(program->window, "Bullet Hell");


	program->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	program->scrtex = SDL_CreateTexture(program->renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// wy≥πczenie widocznoúci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	program->charset = SDL_LoadBMP("./cs8x8.bmp");
	if (spriteErrorHandler(program->charset, "cs8x8.bmp", program)) { return 1; }
	SDL_SetColorKey(program->charset, true, 0x000000);

	program->t1 = SDL_GetTicks();

	return 0;
}

void setupProgramVariables(Program* program) {
	program->t1 = SDL_GetTicks();
	program->frames = 0;
	program->fpsTimer = 0;
	program->fps = 0;
	program->quit = 0;
	program->worldTime = 0;
}

void setPlayerSprites(Program* program, char* n, char* e, char* w, char* s) {
	program->game->playerSprites[N] = program->game->bulletSprite = SDL_LoadBMP(n);
	program->game->playerSprites[E] = program->game->bulletSprite = SDL_LoadBMP(e);
	program->game->playerSprites[W] = program->game->bulletSprite = SDL_LoadBMP(w);
	program->game->playerSprites[S] = program->game->bulletSprite = SDL_LoadBMP(s);

}

void setColors(Program* program) {
	program->palette[BLACK] = SDL_MapRGB(program->screen->format, 0x00, 0x00, 0x00);
	program->palette[GREEN] = SDL_MapRGB(program->screen->format, 0x00, 0xFF, 0x00);
	program->palette[RED] = SDL_MapRGB(program->screen->format, 0xFF, 0x00, 0x00);
	program->palette[BLUE] = SDL_MapRGB(program->screen->format, 0x11, 0x11, 0xCC);
}

int spriteErrorHandler(SDL_Surface *sprite, char* name, Program* program){
	if (sprite == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", name, SDL_GetError());
		SDL_FreeSurface(program->charset);
		SDL_FreeSurface(program->screen);
		SDL_DestroyTexture(program->scrtex);
		SDL_DestroyWindow(program->window);
		SDL_DestroyRenderer(program->renderer);
		SDL_Quit();
		return 1;
	}
	return 0;
}

void newFrame(Program* program) {
		program->t2 = SDL_GetTicks();
		program->delta = (int(program->t2) - int(program->t1)) * 0.001;
		program->t1 = program->t2;

		program->worldTime += program->delta;

		SDL_FillRect(program->screen, NULL, program->palette[BLACK]);
		
		program->fpsTimer += program->delta;
		if(program->fpsTimer > 0.5) {
			program->fps = program->frames * 2;
			program->frames = 0;
			program->fpsTimer -= 0.5;
		};
}

void renderFrame(Program* program) {
		SDL_UpdateTexture(program->scrtex, NULL, program->screen->pixels, program->screen->pitch);
		SDL_RenderCopy(program->renderer, program->scrtex, NULL, NULL);
		SDL_RenderPresent(program->renderer);
}

void updateEntityPosition(Entity* entity, Program* program) {
	float n = 1;
	if (entity->vector[X] * entity->vector[Y]) {
		n = sqrt(pow(entity->vector[X], 2) + pow(entity->vector[Y], 2));
	}
	entity->position[X] += (program->delta * float(entity->vector[X]) * float(entity->speed) / n);
	entity->position[Y] += (program->delta * float(entity->vector[Y]) * float(entity->speed) / n);
}

void updatePlayerPosition(Program* program) {
	float n = 1;
	Entity* entity = program->game->player;
	if (entity->vector[X] * entity->vector[Y]) {
		n = sqrt(pow(entity->vector[X], 2) + pow(entity->vector[Y], 2));
	}
	float dx = (program->delta * float(entity->vector[X]) * float(entity->speed) / n);
	float dy = (program->delta * float(entity->vector[Y]) * float(entity->speed) / n);

	if (!((entity->position[X] < 0 && dx < 0) || (entity->position[X] > program->game->levelWidth && dx > 0))) {
		entity->position[X] += dx;
		program->game->camera[X] += dx;
	}
	
	if (!((entity->position[Y] < 0 && dy < 0) || (entity->position[Y] > program->game->levelheight && dy > 0))) {
		entity->position[Y] += dy;
		program->game->camera[Y] += dy;
	}
		
}

void setEntityPosition(Entity* entity, int x, int y) {
	entity->position[X] = x;
	entity->position[Y] = y;
}

void drawEntity(Entity* entity, Program* program) {
	float x = entity->position[X] - program->game->camera[X];
	float y = entity->position[Y] - program->game->camera[Y];

	if( (x+(entity->sprite->w/2) >= 0 && x - (entity->sprite->w/2) <= SCREEN_WIDTH) || (y + (entity->sprite->h/2) >=0 && y - (entity->sprite->h/2) <= SCREEN_HEIGHT) )
		DrawSurface(program->screen, entity->sprite, x, y);
}

int loadEntitySprite(Entity* entity, char* path, Program *program) {
	entity->sprite = SDL_LoadBMP(path);
	if (!spriteErrorHandler(entity->sprite, path, program)) {
		entity->hitbox[X] = entity->sprite->w / 2;
		entity->hitbox[Y] = entity->sprite->h / 2;
		return 0;
	}
	else {
		return 1;
	}
}

int loadBackground(char* path, Program* program) {
	program->game->background = SDL_LoadBMP(path);
	return spriteErrorHandler(program->game->background, path, program);
}

int setBulletSprite(char* path, Program *program) {
	program->game->bulletSprite = SDL_LoadBMP(path);
	if (spriteErrorHandler(program->game->bulletSprite, path, program)) { return NULL; }
}


int detectCollision(Entity* e1, Entity* e2) {
	if (abs(int(e1->position[X] - e2->position[X])) <= abs(e1->hitbox[X] + e2->hitbox[X]) &&
		abs(int(e1->position[Y] - e2->position[Y])) <= abs(e1->hitbox[Y] + e2->hitbox[Y])) {
			return 1;
		}

	return 0;
}

void setEntityHitbox(Entity* entity, int x, int y) {
	entity->hitbox[X] = x;
	entity->hitbox[Y] = y;
}

Entity* spawnBullet(double ang, int x, int y, Program* program, int speed) {
	double vy = -sin(ang);
	double vx = cos(ang);
	Entity* bullet = new Entity;
	bullet->speed = speed;
	bullet->vector[X] = vx;
	bullet->vector[Y] = vy;
	bullet->position[X] = x;
	bullet->position[Y] = y;
	bullet->movement = STRAIGHT;
	bullet->sprite = program->game->bulletSprite;
	bullet->hitbox[X] = bullet->sprite->w/2;
	bullet->hitbox[Y] = bullet->sprite->h/2;
	bullet->damage = 1;

	return bullet;
}

Entity* spawnPlayerBullet(Program* program, int speed) {
	Entity* player = program->game->player;
	Entity* bullet = new Entity;
	switch (player->direction) {
	case N:
		bullet->vector[X] = 0; bullet->vector[Y] = -1;
		break;
	case S:
		bullet->vector[X] = 0; bullet->vector[Y] = 1;
		break;
	case E:
		bullet->vector[X] = 1; bullet->vector[Y] = 0;
		break;
	case W:
		bullet->vector[X] = -1; bullet->vector[Y] = 0;
		break;
	default:
		return NULL;
	}
	bullet->position[X] = player->position[X];
	bullet->position[Y] = player->position[Y];
	bullet->movement = STRAIGHT;
	bullet->sprite = program->game->bulletSprite;
	bullet->hitbox[X] = bullet->sprite->w / 2;
	bullet->hitbox[Y] = bullet->sprite->h / 2;
	bullet->damage = 1;
	bullet->speed = speed;
	return bullet;
}

Entity* getEnemy(Program* program) {
	Entity* enemy = new Entity;
	*enemy = *program->game->enemyData;
	return enemy;
}

double getPlayerAngle(Entity* entity, Program* program){
	Entity* player = program->game->player;
	return atan2(-1*(player->position[Y] - entity->position[Y]), player->position[X] - entity->position[X]);
}

void setEntityVector(Entity* entity, double ang) {
	entity->vector[X] = cos(ang);
	entity->vector[Y] = -sin(ang);
}

int selectAttack(Entity* entity) {
	if (entity->attack2 == NULL) {
		return entity->attack1;
	}
	else {
		if (rand() % 2 == 0)
			return entity->attack1;
		else
			return entity->attack2;
	}
}

void performAttack(Entity* entity, int attackCode, Program* program){
	double angle = getPlayerAngle(entity, program);
	switch (attackCode) {
	case ONEBULLET: {
		pushEntityToList(program->game->bulletList, spawnBullet(angle, entity->position[X], entity->position[Y], program, entity->speed +25));
		break;
	}
	case CONE3: {
		for (int i = -1; i <= 1 ; i++) {
			pushEntityToList(program->game->bulletList, spawnBullet(angle+(PI*i*0.125), entity->position[X], entity->position[Y], program, entity->speed + 25));
		}
		break;
	}
	case CONE5: {
		for (int i = -2; i <= 2; i++) {
				pushEntityToList(program->game->bulletList, spawnBullet(angle + (PI * i * 0.125), entity->position[X], entity->position[Y], program, entity->speed + 25));
		}
		break;
	}
	case CIRCLE: {
		for (int i = 0; i <= 10; i++) {
			pushEntityToList(program->game->bulletList, spawnBullet((angle + (PI * i * 0.2)), entity->position[X], entity->position[Y], program, entity->speed + 25));
		}
		break;
	}
	}
	entity->cdTimer = 0;
}

char getGrade(int score) {
	char grade = ' ';

	if (score < -5) {
		grade = 'F';
	}
	else if (score < 1) {
		grade = 'E';
	}
	else if (score < 3) {
		grade = 'D';
	}
	else if (score < 8) {
		grade = 'C';
	}
	else if (score < 11) {
		grade = 'B';
	}
	else if (score < 15) {
		grade = 'A';
	}
	else if (score >= 15) {
		grade = 'S';
	}

	return grade;
}

void performMovement(Entity* entity, int movementCode, Program* program) {
	double ang = getPlayerAngle(entity, program);
	int flag = 0;
	switch (movementCode) {
	case STATIC:
		break;
	case STRAIGHT:
		updateEntityPosition(entity, program);
		break;
	case FOLLOW:
		setEntityVector(entity, ang);
		updateEntityPosition(entity, program);
	case BOUNCE:
		updateEntityPosition(entity, program);
		
		if (entity->position[X] <= 0 || entity->position[X] >= program->game->levelWidth) {
			entity->vector[X] *= -1;
			flag = 1;
		}
		if (entity->position[Y] <= 0 || entity->position[Y] >= program->game->levelheight) {
			entity->vector[Y] *= -1;
			flag = 1;
		}
		if (flag) {
			updateEntityPosition(entity, program);
			updateEntityPosition(entity, program);
		}
		break;
	default: break;
	}
}

void shoot(Program* program) {
	program->game->player->cdTimer = 0;
	pushEntityToList(program->game->playerBullets, spawnPlayerBullet(program, 500));
}

void drawInterface(Program* program) {
	char* text = program->text;
		DrawRectangle(program->screen, 4, 4, SCREEN_WIDTH - 8, 36, program->palette[GREEN], program->palette[BLACK]);
		sprintf(text, "Time: %.1lf s FPS: %.0lf Score: %i", program->worldTime, program->fps, program->game->score);
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 10, text, program->charset);
		
		char playerData[PLAYER_HEALTH+3];
		for (int i = 0; i < PLAYER_HEALTH; i++) {
			if (program->game->player->health > i) { playerData[i] = '\003'; }
			else { playerData[i] = '\007'; }
		}
		playerData[PLAYER_HEALTH] = ' ';
		if (program->game->player->cdTimer >= program->game->player->cdTime) { playerData[PLAYER_HEALTH + 1] = '\006'; }
		else { playerData[PLAYER_HEALTH + 1] = '\007'; }
		playerData[PLAYER_HEALTH + 2] = '\0';

		sprintf(text, "Health: %s", playerData);
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 26, text, program->charset);
		
}

int dealDamage(Entity* entity, Entity* target) {
	if (target->killable && target->iframeTimer >= IFRAMES) {
		target->health -= entity->damage;
		target->iframeTimer = 0;
		return 1;
	}
	return 0;
}

void gameOverLoop(Program* program) {
	int nextlevel = NULL;
	while (program->state == GAMEOVER && !program->quit) {
		newFrame(program);

		char* text = program->text;
		DrawRectangle(program->screen, 25, 25, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, program->palette[GREEN], program->palette[BLACK]);
		//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
		
		if (program->game->player->health <= 0) {
			sprintf(text, "Porazka!");
		}
		else {
			sprintf(text, "Zwyciestwo!");
		}
		
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 45, text, program->charset);

		sprintf(text, "Wynik: %i  Ranga: %c", program->game->score, getGrade(program->game->score));
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 76, text, program->charset);

		sprintf(text, "Wcisnij \"ENTER\" aby sprobowac jeszcze raz.");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 106, text, program->charset);

		sprintf(text, "Wcisnij \"BACKSPACE\" aby wrocic do menu.");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 136, text, program->charset);

		if (program->game->player->health > 0) {
			sprintf(text, "Wcisnij \"N\" aby przejsc do kolejnego poziomu.");
			DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 166, text, program->charset);
		}

		renderFrame(program);
		SDL_Event *event = &(program->event);
		while(SDL_PollEvent(event)) {
			switch (event->type) {
			case SDL_KEYDOWN:
					if(event->key.keysym.sym == SDLK_ESCAPE) program->quit = 1;
					else if (event->key.keysym.sym == SDLK_BACKSPACE) { program->state = MENU;  menuLoop(program); }
					else if (event->key.keysym.sym == SDLK_RETURN) { cleanupLevel(program); startLevel(program); }
					else if (event->key.keysym.sym == SDLK_n && program->game->player->health > 0) { cleanupLevel(program); selectLevel(program->game->levelId + 1, program); }
					break;
				case SDL_QUIT:
					program->quit = 1;
					break;
				};
			};
	}
}

void cleanupLevel(Program* program) {
	clearList(program->game->bulletList);
	clearList(program->game->healthPickupList);
	clearList(program->game->playerBullets);
}

void drawBackground(Program* program) {
	DrawSurface(program->screen, program->game->background, (program->game->levelWidth/2)-program->game->camera[X],(program->game->levelheight/2)-program->game->camera[Y]);
}

void drawBorder(Program* program) {
	float cx = program->game->player->position[X];
	float cy = program->game->player->position[Y];
	if (cx - SCREEN_WIDTH / 2 < 0)
		DrawLine(program->screen, SCREEN_WIDTH/2-cx, 0, SCREEN_HEIGHT, 0,1, program->palette[GREEN]);
	if(cx + SCREEN_WIDTH / 2 > program->game->levelWidth)
		DrawLine(program->screen, program->game->levelWidth - cx - (SCREEN_WIDTH / 2), 0, SCREEN_HEIGHT, 0, 1, program->palette[GREEN]);
	if (cy - SCREEN_HEIGHT / 2 < 0)
		DrawLine(program->screen, 0, SCREEN_HEIGHT / 2 - cy,  SCREEN_WIDTH, 1, 0, program->palette[GREEN]);
	if (cy + SCREEN_HEIGHT / 2 > program->game->levelheight){
		DrawLine(program->screen, 1, program->game->levelheight - cy + (SCREEN_HEIGHT / 2) , SCREEN_WIDTH, 1, 0, program->palette[GREEN]);
	}
		
}

void gameLoop(Program* program){
	while (program->state == LEVEL && !program->quit) {
		newFrame(program);

		drawBackground(program);
		drawBorder(program);
		Entity* player = program->game->player;
		Game* game = program->game;

		updatePlayerPosition(program);
		player->cdTimer += program->delta;
		player->iframeTimer += program->delta;
		game->pickupRespawnTimer += program->delta;

		//spawn health pickups
		if (game->pickupRespawnTimer >= game->pickupRespawn) {
			Entity* pickup = new Entity;
			*pickup = *game->healthPickup;
			setEntityPosition(pickup, rand()%game->levelWidth, rand()%game->levelheight);
			pushEntityToList(game->healthPickupList, pickup);
			game->pickupRespawnTimer = 0;
		}

		//process pickups
		EntityListNode* node = game->healthPickupList;
		while (node) {
			if (node->entity) {
				drawEntity(node->entity, program);
				if (detectCollision(node->entity, player)) {
					if (player->health < player->maxhealth) {
						player->health += 1;
						game->score += 1;
					}
					EntityListNode* tempNode = node->next;
					removeNode(node);
					node = tempNode;
				}
				else { node = node->next; }
			}
			else { node = node->next; }
		}
		
		//process bullets
		node = program->game->bulletList;
		while (node) {
			if(node->entity){
			performMovement(node->entity, node->entity->movement, program);
			drawEntity(node->entity, program);
			//updateEntityPosition(node->entity, program);
			if (detectCollision(node->entity, player)) {
				if (dealDamage(node->entity, player)) {
					game->score -= game->negativeMultiplier;
					game->negativeMultiplier *= 2;
					game->positiveMultiplier = 1;
				}
				EntityListNode* tempNode = node->next;
				removeNode(node);
				node = tempNode;
			}else if (	node->entity->position[X] < 0 ||
						node->entity->position[X] > game->levelWidth ||
						node->entity->position[Y] < 0 ||
						node->entity->position[Y] > game->levelheight) {
				EntityListNode* tempNode = node->next;
				removeNode(node);
				node = tempNode;
			}else { node = node->next; }
			}else{ node = node->next; }
		}
		
		Entity* enemy = game->enemyData;
		//process player bullets
		node = program->game->playerBullets;
		while (node) {
			if (node->entity) {
				performMovement(node->entity, node->entity->movement, program);
				drawEntity(node->entity, program);
				node->entity->iframeTimer += program->delta;
				if (detectCollision(node->entity, enemy)) {
					if (dealDamage(node->entity, enemy)) {
							game->score += game->positiveMultiplier;
							game->positiveMultiplier *= 2;
							game->negativeMultiplier = 1;
					}
					EntityListNode* tempNode = node->next;
					removeNode(node);
					node = tempNode;
				}
				else { node = node->next; }
			}
			else { node = node->next; }
		}

		//process enemies
		
		if (enemy) {
			performMovement(enemy, enemy->movement, program);
			drawEntity(enemy, program);
			enemy->cdTimer += program->delta;
			enemy->iframeTimer += program->delta;
			if (enemy->cdTimer >= enemy->cdTime) {
				performAttack(enemy, selectAttack(enemy), program);
			}
		}

		//check if player lost
		if (player->health <= 0 || enemy->health <= 0) {
			program->state = GAMEOVER;
		}

		drawEntity(player, program);
		drawInterface(program);
		renderFrame(program);

		// obs≥uga zdarzeÒ (o ile jakieú zasz≥y) / handling of events (if there were any)
		SDL_Event *event = &(program->event);
		while(SDL_PollEvent(event)) {
			switch (event->type) {
			case SDL_KEYDOWN:
					if(event->key.keysym.sym == SDLK_ESCAPE) program->quit = 1;
					else if (event->key.keysym.sym == SDLK_UP) { player->vector[Y] = -1; player->direction = N; player->sprite = game->playerSprites[N]; }
					else if (event->key.keysym.sym == SDLK_DOWN) { player->vector[Y] = 1; player->direction = S; player->sprite = game->playerSprites[S]; }
					else if (event->key.keysym.sym == SDLK_RIGHT) { player->vector[X] = 1; player->direction = E; player->sprite = game->playerSprites[E]; }
					else if (event->key.keysym.sym == SDLK_LEFT) { player->vector[X] = -1; player->direction = W; player->sprite = game->playerSprites[W]; }
					else if (event->key.keysym.sym == SDLK_SPACE) {
						if (player->cdTimer >= player->cdTime) {
 							shoot(program);
						}
					}
					else if (event->key.keysym.sym == SDLK_n) { cleanupLevel(program); startLevel(program); }
					break;
				case SDL_KEYUP:
					if (event->key.keysym.sym == SDLK_ESCAPE) program->quit = 1;
					else if (event->key.keysym.sym == SDLK_UP && player->vector[Y]== -1) player->vector[Y] = 0;
					else if (event->key.keysym.sym == SDLK_DOWN && player->vector[Y] == 1)  player->vector[Y] = 0;
					else if (event->key.keysym.sym == SDLK_RIGHT && player->vector[X] == 1) player->vector[X] = 0;
					else if (event->key.keysym.sym == SDLK_LEFT && player->vector[X] == -1)  player->vector[X] = 0;
					break;
				case SDL_QUIT:
					program->quit = 1;
					break;
				};
			};
		program->frames++;
	}
	cleanupLevel(program);
	gameOverLoop(program);
}

void startLevel(Program* program) {
	Game* game = program->game;
	program->worldTime = 0;
	program->frames = 0;
	program->state = LEVEL;
	game->score = 0;
	game->positiveMultiplier = 1;
	game->negativeMultiplier = 1;
	game->player->health = game->player->maxhealth;
	game->enemyData->health = game->enemyData->maxhealth;
	game->pickupRespawnTimer = 0;
	setEntityPosition(game->player, game->levelWidth/2, game->levelheight/2);
	setEntityPosition(game->enemyData, game->player->position[X] + (rand() % 300) - 150, game->player->position[Y] + (rand() % 300) - 150);
	game->camera[X] = float(game->levelWidth / 2 - SCREEN_WIDTH / 2);
	game->camera[Y] = float(game->levelheight / 2 - SCREEN_HEIGHT / 2);

	gameLoop(program);
}

void levelSetUp(char* background, int pickupRespawn, Program* program) {
	loadBackground(background, program);
	program->game->pickupRespawn = pickupRespawn;
	program->game->levelWidth = program->game->background->w;
	program->game->levelheight = program->game->background->h;
}

void selectLevel(int levelId, Program* program) {
	program->game->levelId = levelId;
	
	Entity enemy;
	if (levelId <= LEVELS) {
		program->state = LEVEL;
		switch (levelId) {
		case 1:
			levelSetUp("./space.bmp", 3, program);
			//starfish
			enemy = createEnemy("./starFishA.bmp", program, STATIC, CIRCLE, NULL, 2, 60, 5);
			break;
		case 2:
			levelSetUp("./space2.bmp", 3, program);
			//starfish
			enemy = createEnemy("./starFishB.bmp", program, FOLLOW, CONE5, NULL, 2, 85, 5);
			break;
		case 3:
			levelSetUp("./space3.bmp", 3, program);
			enemy = createEnemy("./starFishC.bmp", program, BOUNCE, CONE3, CONE5, 2, 85, 5);
			setEntityVector(&enemy, getPlayerAngle(&enemy, program));
			break;
		default:
			return;
		}
		program->game->enemyData = &enemy;
		startLevel(program);
	}
}

void menuLoop(Program* program) {
	while (program->state == MENU && !program->quit) {
		newFrame(program);

		char* text = program->text;
		DrawRectangle(program->screen, 25, 25, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, program->palette[GREEN], program->palette[BLACK]);
		sprintf(text, "Wybierz Poziom");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 45, text, program->charset);

		sprintf(text, "1. Atak Kosmicznych Rozgwiazd");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 66, text, program->charset);

		sprintf(text, "2. Uciec przed szkarlupniem");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 86, text, program->charset);

		sprintf(text, "3. Mokra robota");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 106, text, program->charset);

		sprintf(text, "ESC: Wyjdz z gry");
		DrawString(program->screen, program->screen->w / 2 - strlen(text) * 8 / 2, 126, text, program->charset);

		renderFrame(program);

		SDL_Event* event = &(program->event);
		while (SDL_PollEvent(event)) {
			switch (event->type) {
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_1) {
					selectLevel(1, program);
				}
				else if (event->key.keysym.sym == SDLK_2) {
					selectLevel(2, program);
				}
				else if (event->key.keysym.sym == SDLK_3) {
					selectLevel(3, program);
				}
				if (event->key.keysym.sym == SDLK_ESCAPE) { program->quit = 1; }
				break;
			case SDL_QUIT:
				program->quit = 1;
				break;
			}
		}
	}
}



Entity createEnemy(char* sprite, Program* program, int movement, int attack1, int attack2, int cdTime, int speed, int health) {
	Entity enemy;
	enemy.attack1 = attack1;
	enemy.attack2 = attack2;
	enemy.movement = movement;
	enemy.speed = speed;
	enemy.maxhealth = health;
	enemy.health = health;
	enemy.cdTime = cdTime;
	loadEntitySprite(&enemy, sprite, program);
	return enemy;
}


// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	Program* program = new Program;
	srand(time(NULL));
	if (setupProgram(program)) { return 1; }
	setColors(program);
	setupProgramVariables(program);

	setPlayerSprites(program, "./statekN.bmp", "./statekE.bmp", "./statekW.bmp", "./statekS.bmp");
	setBulletSprite("./bullet.bmp", program);

	program->game->healthPickup = new Entity;
	if (loadEntitySprite(program->game->healthPickup, "./health.bmp", program)) { return 1; }

	Entity* player = new Entity;
	if (loadEntitySprite(player, "./statekN.bmp", program)) { return 1; };
	player->speed = 350;
	program->game->player = player;

	while (!program->quit) {
		menuLoop(program);
		
	};

	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(program->charset);
	SDL_FreeSurface(program->screen);
	SDL_DestroyTexture(program->scrtex);
	SDL_DestroyRenderer(program->renderer);
	SDL_DestroyWindow(program->window);


	//zwolnieni pamiÍci
	clearList(program->game->bulletList);
	clearList(program->game->playerBullets);
	clearList(program->game->healthPickupList);

	SDL_Quit();
	return 0;
	};
