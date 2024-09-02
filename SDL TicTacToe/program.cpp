#pragma region C++ Includes
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#pragma endregion

#pragma region SDL Includes
#include <SDL.h>
#pragma endregion

#pragma region Engine Includes
#include "Input.h"
#include "Random.h"
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#include "TicTacToeGame.h"
#pragma endregion

#pragma region Emscripten Includes
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#pragma endregion

using namespace std;
using namespace std::chrono;

#pragma region Constant Parameters
/*
 * Viewport should be Full-HD and full screen
 * in release builds, and windowed HD-Ready
 * in debug builds, to ease debug operations.
 */
#ifdef _DEBUG
#define VIEWPORT_W 1280
#define VIEWPORT_H 720
#define VIEWPORT_MODE SDL_WINDOW_RESIZABLE
#else
#ifdef __EMSCRIPTEN__
#define VIEWPORT_W 800
#define VIEWPORT_H 450
#define VIEWPORT_MODE SDL_WINDOW_RESIZABLE
#elif
#define VIEWPORT_W 1920
#define VIEWPORT_H 1080
#define VIEWPORT_MODE SDL_WINDOW_FULLSCREEN
#endif
#endif
#define SDL_INIT_MODE (SDL_INIT_VIDEO | SDL_INIT_AUDIO)

#define TARGET_FPS 60
#define TARGET_FRAME_TIME (1000 / TARGET_FPS)

#define RENDER_CLEAR_COLOR 10, 10, 10, 255

#ifdef __EMSCRIPTEN__
//	Web container interaction
#define HTML_CANVAS_SELECTOR "#canvas"
#endif

//	Command line arguments
#define CLI_KEY_CROSS_FULL "-cross"
#define CLI_KEY_CROSS "-x"
#define CLI_KEY_CIRCLE_FULL "-circle"
#define CLI_KEY_CIRCLE "-o"
#define CLI_VAL_CPU_EASY "easy"
#define CLI_VAL_CPU_MEDIUM "medium"
#define CLI_VAL_CPU_HARD "hard"
#pragma endregion

#define AI_TIME 250

#pragma region Exchange data
typedef struct
{
	SDL_Window * window;
	SDL_Renderer * r;
	SDL_Rect viewport;

} SystemData;
typedef struct
{
	bool closeRequested;
	vector<IUpdatable *> updateQueue;
	vector<IRenderable *> renderQueue;
} EngineData;
typedef struct
{
	TicTacToeGame * ticTacToeGame;
} GameData;
typedef struct
{
	SystemData system;
	EngineData engine;
	GameData game;
} Context;
#pragma endregion

//	Forward declarations
void RefreshViewportSize();
int SystemSetup();
void MainLoop();
void SystemShutdown();
void OverrideControl(int argc, char * argv[], const char * argCheck, ControlType & control);

//	Prepare a global context for the main loop and the main function
Context ctx;

/*	ENTRY POINT	*/
int main(int argc, char *argv[])
{
#pragma region System Setup
	/*
	 * Here we're going to initialize and set up
	 * the main elements that will make our game
	 * work, such as SDL itself, the game window
	 * and the renderer.
	 */
	const int setupResult = SystemSetup();
	if(setupResult != 0)
		return setupResult;
#pragma endregion

#pragma region Gameplay Setup
	//	Prepare control type for factions
	ControlType crossControlType = CT_Human;
	ControlType circleControlType = CT_Human;

	//	Override control type if command line arguments are passed
	OverrideControl(argc, argv, CLI_KEY_CROSS_FULL, crossControlType);
	OverrideControl(argc, argv, CLI_KEY_CROSS, crossControlType);
	OverrideControl(argc, argv, CLI_KEY_CIRCLE_FULL, circleControlType);
	OverrideControl(argc, argv, CLI_KEY_CIRCLE, circleControlType);
	ctx.game.ticTacToeGame = new TicTacToeGame
	{
		ctx.system.viewport,
		crossControlType,
		circleControlType
	};

	ctx.engine.updateQueue.push_back(ctx.game.ticTacToeGame);
	ctx.engine.renderQueue.push_back(ctx.game.ticTacToeGame);
#pragma endregion

#pragma region Main Loop
	/*
	 * Just a couple of lines, here the program will
	 * spend 99% of its time.
	 * Here all game logic will hook and run, frame
	 * by frame.
	 */
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(MainLoop, 0, 1);
#else
	while(!ctx.engine.closeRequested)
		MainLoop();
#endif
#pragma endregion

#pragma region System Shutdown
	/*
	 * Not calling when building with emscripten,
	 * it follows a slightly different logic.
	 * The main loop started above with
	 *	emscripten_set_main_loop()
	 * schedules a call to the MainLoop function
	 * at intervals decided by the browser (but
	 * configured by us) but that function doesn't
	 * block execution so we don't want to shutdown
	 * the system before the first frame is
	 * processed.
	 * We'll call SystemShutdown() later, in the
	 * main loop.
	 */
#ifndef __EMSCRIPTEN__
	SystemShutdown();
#endif
#pragma endregion

	return 0;
}

void RefreshViewportSize()
{
#ifndef __EMSCRIPTEN__
#ifndef _DEBUG
	SDL_DisplayMode displayMode;
#endif
	//	Use the current window's size, if a window has been already initialized
	if(ctx.system.window)
		SDL_GetWindowSize(ctx.system.window, &ctx.system.viewport.w, &ctx.system.viewport.h);
	else
	//	No window present, so let's get desired window size
#ifndef _DEBUG
	//	Match display's size when in release mode (if it is possible to get the display size)
	if(SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
	{
		ctx.system.viewport.w = displayMode.w;
		ctx.system.viewport.h = displayMode.h;
	}
	else
#endif
	{
		//	Fallback to hardcoded values
		ctx.system.viewport.w = VIEWPORT_W;
		ctx.system.viewport.h = VIEWPORT_H;
	}
#else
	//	When targetting webgl, try to always match the canvas' size
	emscripten_get_canvas_element_size(HTML_CANVAS_SELECTOR, &ctx.system.viewport.w, &ctx.system.viewport.h);
#endif
}

int SystemSetup()
{

	//	Initialize SDL (here we can selectively initialize different modules using SDL_INIT_* OR'd constants)
	if(SDL_Init(SDL_INIT_MODE) < 0)
	{
		cout << "Couldn't initialize SDL2: " << SDL_GetError() << endl;
		return -1;
	}

	//	Perform first window size refres, when no window is present it sets values to the desired size
	RefreshViewportSize();

	//	Create the game window
	ctx.system.window = SDL_CreateWindow(
		"SDL Tic-Tac-Toe!",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ctx.system.viewport.w, ctx.system.viewport.h,
		VIEWPORT_MODE
	);
	if(!ctx.system.window)
	{
		cout << "Couldn't create SDL window: " << SDL_GetError() << endl;
		return -1;
	}

	//	Get or create a rendeer for future render operations
	ctx.system.r = SDL_GetRenderer(ctx.system.window);
	if(!ctx.system.r)
	{
#ifdef _DEBUG
		cout << "Couldn't get SDL renderer from window: " << SDL_GetError() << endl;
		cout << "Trying to create a new renderer.." << endl;
#endif
		ctx.system.r = SDL_CreateRenderer(ctx.system.window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
		if(!ctx.system.r)
		{
			cout << "Couldn't create SDL renderer on window: " << SDL_GetError() << endl;
			return -1;
		}
#ifdef _DEBUG
		else
			cout << "Renderer created succesfully!" << endl;
#endif
	}
#ifdef _DEBUG
	else
		cout << "Renderer retrieved succesfully!" << endl;
#endif

	return 0;
}

void MainLoop()
{
#pragma region Prepare FPS Regulation
	/*
	 * To keep a steady frame rate, we need to know
	 * how long it takes for a frame to make all
	 * calculations and render and then wait for the
	 * target frame time.
	 * To do so, when the frame starts, i.e. when
	 * entering the main 
	 * 
	 * When building for webgl, we let the browser decide
	 * the frame rate, which will typically match the
	 * monitor's refresh rate, so we're skipping all the
	 * framerate regulation stuff when targetting webgl.loop, we store the precise
	 * time.
	 */
#ifndef __EMSCRIPTEN__
	steady_clock::time_point frameStart = high_resolution_clock::now();
#endif
#pragma endregion

#pragma region Events/Input Loop
	/*
	 * Here we decided to move all the event loop to a
	 * dedicated class, the Input class.
	 * This may not the best choice, depending on the
	 * scope and the goals of the project.
	 * Read notes above the Input::PollEvents method for
	 * more considerations.
	 */

	Input::Get().PollEvents();

	//	Handle quit requests
	if(Input::Get().WasQuitRequested())
		ctx.engine.closeRequested = true;
#pragma endregion

#pragma region Update Loop (Logic)
	for(IUpdatable *& updatable : ctx.engine.updateQueue)
		updatable->Update();
#pragma endregion

#pragma region Render Loop
	//	Adapt viewport to the window
	RefreshViewportSize();

	//	Send a pre-render message to all subscribers so they can prepare for rendering
	for(IRenderable * const & renderable : ctx.engine.renderQueue)
		renderable->PreRender(ctx.system.r);

	//	Let's clear the canvas before drawing a new frame
	SDL_SetRenderDrawColor(ctx.system.r, RENDER_CLEAR_COLOR);
	SDL_RenderClear(ctx.system.r);

	//	Draw all renderables to the back buffer (Render is a const function)
	for(const IRenderable * const & renderable : ctx.engine.renderQueue)
		renderable->Render(ctx.system.r);

	//	Swap front and back buffer to show results of the render
	SDL_RenderPresent(ctx.system.r);
#pragma endregion

#pragma region FPS Regulation
	/*
	 * We calculate the frame time, relative to the frame start time.
	 * If it's below the target frame time, we'll wait for the difference.
	 * If, instead, the frame take longer than the frame time we have two
	 * roads to walk:
	 * - variable frame time: we just don't wait and rush into the next frame
	 * - fixed frame time: we skip a frame to align to the next fixed frame time
	 *
	 * Important note: when working with high resolution clock (or numbers
	 * in general), data is usually stored in large data formats such as
	 * long int or even long long int, so it's good practice to avoid
	 * implicit conversions to make 100% clear what type of data we're
	 * carying around.
	 * 
	 * As stated above, FPS regulation is entrusted to the browser for
	 * webgl builds, so we'll skip the manual frame rate regulation here
	 * too.
	 */
#ifndef __EMSCRIPTEN__
	long long elapsedMillis = duration_cast<milliseconds>(high_resolution_clock::now() - frameStart).count();
#ifdef FRAME_SKIP
	elapsedMillis %= TARGET_FPS;
#endif
	long long waitMillis = (1000 / TARGET_FPS) - elapsedMillis;
	if(waitMillis > 0)
		SDL_Delay((int)waitMillis);
#endif
#pragma endregion

#pragma region WebGL Shutdown
	/*
	 * When targetting webgl this function (MainLoop) is
	 * called again and again by the browser. When the
	 * game requests a close, we need to shutdown the
	 * system.
	 */
#ifdef __EMSCRIPTEN__
	if(ctx.engine.closeRequested)
		SystemShutdown();
#endif
#pragma endregion
}

void SystemShutdown()
{
	/*
	 * Closing operations are very much important.
	 * Always remember to clean all created objects
	 * and to quit all the systems.
	 * This is especially useful when the same
	 * application opens and closes the resources
	 * multiple times in the same run.
	 *
	 * Specific for emscripten, we stop the
	 * main loop.
	 */
#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif

	//	Dispose the game
	if(ctx.game.ticTacToeGame)
	{
		delete ctx.game.ticTacToeGame;
		ctx.game.ticTacToeGame = nullptr;
	}

	//	Quit all systems
	SDL_DestroyRenderer(ctx.system.r);
	SDL_DestroyWindow(ctx.system.window);
	SDL_Quit();
}

void OverrideControl(int argc, char * argv[], const char * argCheck, ControlType & controlType)
{
	/*
	 * Iterate command line arguments and look for an argument
	 * matching the argCheck parameter, followed by a difficulty
	 * value.
	 * If found, override the control type.
	 */
	for(int a = 0; a < argc; a++)
		if(
			strcmp(argv[a], argCheck) == 0 &&
			a < argc - 1
		)
		{
			if(strcmp(argv[a + 1], CLI_VAL_CPU_EASY) == 0)
				controlType = CT_CPU_Easy;
			else if(strcmp(argv[a + 1], CLI_VAL_CPU_MEDIUM) == 0)
				controlType = CT_CPU_Medium;
			if(strcmp(argv[a + 1], CLI_VAL_CPU_HARD) == 0)
				controlType = CT_CPU_Hard;
		}
}
