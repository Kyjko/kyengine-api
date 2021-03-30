#include <SDL.h>
#include <stdio.h>
#include <memory.h>
#include <glew.h>
#include <time.h>
#undef main

#define kyMain				main

#define CALLBACK
#define API
#define APIENTRY			__stdcall

#define CENTER				SDL_WINDOWPOS_CENTERED
#define VSYNC				SDL_RENDERER_PRESENTVSYNC
#define OPENGL				SDL_WINDOW_OPENGL

#define DEFAULT_ENTITY_NUM	100

typedef void				VOID;
typedef void*				VOIDPTR;
typedef struct context_s*	CTXHNDL;
typedef _Bool				BOOL;
typedef char*				STR;
typedef char				CHR;
typedef int					ERRN;
typedef int					DWRD;
typedef unsigned int		UINT;
typedef short				WRD;
typedef long				LNG;
typedef long long			LLNG;
typedef float				SPFLT;
typedef double				DPFLT;
typedef SDL_Window*			WNDHNDL;
typedef SDL_Renderer*		RNDRHNDL;
typedef SDL_GLContext		GLCTX;

#define EXITSUCCESS			0
#define	EXITFAIL			-1

typedef enum log_e {
	INFO,
	WARN,
	ERROR,
	FATAL
} log_e;

typedef struct vertex_s {
	SPFLT x, y;
	SPFLT r, g, b, a;
} vertex_s;

typedef struct entity_s {
	UINT id;
	vertex_s* vertices;
} Entity;

typedef struct context_s {
	WNDHNDL w;
	RNDRHNDL r;
	GLCTX glctx;
	STR title;
	DWRD W;
	DWRD H;
	DWRD wPosX;
	DWRD wPosY;
	DWRD wFlags;
	DWRD rFlags;
	DWRD initErr;
	BOOL quit;
	UINT defaultentitynum;
	CALLBACK VOID(*render)(CTXHNDL);
	CALLBACK VOID(*callback)(CTXHNDL, VOID(*)(CTXHNDL));
	CALLBACK VOID(*initentities)(CTXHNDL);
} CTX;

Entity* entities;
UINT _inner_entity_count;
UINT* vbos;
UINT* vaos;
UINT* ibos;

API inline
VOID Init(CTXHNDL);

CALLBACK static
VOID Callback(CTXHNDL ,VOID(*render)(CTXHNDL));

API inline
VOID DeInit(CTXHNDL);

API inline
VOID Log(STR, log_e);

API
VOID AllocateEntity(Entity*);

API
VOID DeAllocateEntites(Entity*);

API
VOID RenderEntites(VOID);

API
VOID UpdateEntities(VOID);

API inline
VOID Log(STR msg, log_e type) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%s [%02d:%02d:%02d] %s\n", (type == INFO) ? "[I]" 
		: (type == WARN) ? "[-]" : (type == ERROR) ? "[!]" 
		: "[!!]", tm.tm_hour, tm.tm_min, tm.tm_sec, msg);
}

API
VOID AllocateEntity(Entity* e) {
	entities[_inner_entity_count++] = *e;
	if (_inner_entity_count > DEFAULT_ENTITY_NUM - 1) {
		_inner_entity_count--;
	}
}

API
VOID DeAllocateEntities(Entity* e) {
	free(e);
	e = NULL;
}

API
VOID RenderEntites(VOID) {
	for (int i = 0; i < _inner_entity_count; i++) {

	}
}

API
VOID UpdateEntities(VOID) {
	for (int i = 0; i < _inner_entity_count; i++) {

	}
}

API inline
VOID Init(CTXHNDL ctx) {
	_inner_entity_count = 0;
	entities = (Entity*)malloc(sizeof(Entity)*DEFAULT_ENTITY_NUM);
	ctx->initentities(ctx);
	ctx->callback = Callback;
	ERRN err = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		err = -1;
		printf("%s\n", SDL_GetError());
	}
	ctx->w = SDL_CreateWindow(ctx->title, ctx->wPosX, ctx->wPosY, ctx->W, ctx->H, ctx->wFlags);
	if (ctx->w == NULL) {
		err = -2;
		printf("%s\n", SDL_GetError());
	}
	ctx->r = SDL_CreateRenderer(ctx->w, -1, ctx->rFlags);
	if (ctx->r == NULL) {
		err = -3;
		printf("%s\n", SDL_GetError());
	}

	ctx->glctx = SDL_GL_CreateContext(ctx->w);
	if (ctx->glctx == NULL) {
		err = -4;
		printf("Cannot create glew context!\n");
	}
	if (glewInit() != GLEW_OK) {
		err = -5;
		printf("Cannot initialize GLEW!\n");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);
	glClearColor(0, 0, 0, 0);

	ctx->initErr = err;
	if (err == 0) {
		Log("Context initialized", INFO);
		ctx->callback(ctx, ctx->render);
	}
	else {
		Log("Couldn't initialize context!", FATAL);
	}
}

API inline
VOID DeInit(CTXHNDL ctx) {
	memset(ctx, 0, sizeof(*ctx));
	ctx = (VOIDPTR)NULL;
	DeAllocateEntities(entities);
	Log("Context destroyed", INFO);
}


CALLBACK static
VOID Callback(CTXHNDL ctx, VOID(*render)(CTXHNDL)) {
	while (!ctx->quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				ctx->quit = 1;
				break;
			default: {}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render(ctx);
		SDL_GL_SwapWindow(ctx->w);
	}
	SDL_DestroyRenderer(ctx->r);
	SDL_DestroyWindow(ctx->w);
	ctx->w = NULL;
	ctx->r = NULL;
	ctx->glctx = NULL;
	SDL_Quit();

}