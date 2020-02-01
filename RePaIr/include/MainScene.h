#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/PointLight.h"
#include "Billboard.h"

class MainScene : public Cappuccino::Scene {
public:
	MainScene(bool yn);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;
	void mouseFunction(double xpos, double ypos) override;


private:
	Cappuccino::Camera c;
	Cappuccino::Shader _billboardShader{ "billboardShader.vert","billboardShader.frag" };
	Cappuccino::PointLight _pLight;

	Billboard* _box;
	std::vector<Billboard*> _lightCubes;

};