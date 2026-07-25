#include "Player.h"
#include "Component.h"
#include "GameObject.h"

void Player::Update(float fElapsedTime) {
	if ((FlowEngine::Engine::Get_Key(olc::Key::A).bHeld && wasd) || (!wasd && FlowEngine::Engine::Get_Key(olc::Key::LEFT).bHeld)) {
		_gameObject->pos += olc::vf2d(-speed * fElapsedTime, 0);
	}
	else if ((FlowEngine::Engine::Get_Key(olc::Key::S).bHeld && wasd) || (!wasd && FlowEngine::Engine::Get_Key(olc::Key::DOWN).bHeld)) {
		_gameObject->pos += olc::vf2d(0, speed * fElapsedTime);
	}
	else if ((FlowEngine::Engine::Get_Key(olc::Key::D).bHeld && wasd) || (!wasd && FlowEngine::Engine::Get_Key(olc::Key::RIGHT).bHeld)) {
		_gameObject->pos += olc::vf2d(speed * fElapsedTime, 0);
	}
	else if ((FlowEngine::Engine::Get_Key(olc::Key::W).bHeld && wasd) || (!wasd && FlowEngine::Engine::Get_Key(olc::Key::UP).bHeld))
	{
		_gameObject->pos += olc::vf2d(0, -speed * fElapsedTime);
	}
	else if (FlowEngine::Engine::Get_Key(olc::Key::V).bHeld)
	{
		speed++;
	}
	else if (FlowEngine::Engine::Get_Key(olc::Key::C).bHeld)
	{
		speed--;
	}
	else if (FlowEngine::Engine::Get_Key(olc::Key::N).bHeld)
	{
		_gameObject->scale.x -= 1;
		_gameObject->scale.y -= 1;
	}
	else if(FlowEngine::Engine::Get_Key(olc::Key::M).bHeld)
	{
		_gameObject->scale.x += 1;
		_gameObject->scale.y += 1;
	}
}

void Player::Render(FlowEngine::Engine* engine) {
	olc::vf2d desPos = _gameObject->pos - olc::vf2d(_gameObject->scale.x / 2, _gameObject->scale.y / 2);

	for (float y = 0; y < _gameObject->scale.y; y++) {
		for (float x = 0; x < _gameObject->scale.x; x++)
		{
			engine->Draw(desPos.x + x, desPos.y + y);
		}
	}
}