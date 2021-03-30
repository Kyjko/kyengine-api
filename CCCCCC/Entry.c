#include "kyapi.h"

VOID Render() {
	UpdateEntities();
	RenderEntites();
}

VOID InitEntities(CTXHNDL ctx) {
	Entity a;
	AllocateEntity(&a);
}

DWRD APIENTRY kyMain(VOID) {
	CTX ctx;
	memset(&ctx, 0, sizeof(ctx));
	ctx.title = "Test KyAPI Application";
	ctx.W = 1920;
	ctx.H = 1080;
	ctx.wPosX = CENTER;
	ctx.wPosY = CENTER;
	ctx.wFlags = OPENGL;
	ctx.rFlags = VSYNC;
	ctx.render = Render;
	ctx.initentities = InitEntities;
	ctx.quit = 0;

	Init(&ctx);
	if (ctx.initErr != 0) {
		exit(-1);
	}
	DeInit(&ctx);

	return EXITSUCCESS;
}