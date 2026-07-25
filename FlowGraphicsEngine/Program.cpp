#define OLC_PGE_APPLICATION
#include "Engine.h"
#include <memory>
#include "Object.h"
#include "GameObject.h"
#include "Player.h"

int main() {
	FlowEngine::Engine engine;
	if (engine.Construct(1920, 1080, 1, 1, true)) {
		FlowEngine::
		engine.Start();
	}
	return 0;
}