#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappInput.h"
#include "Cappuccino/PointLight.h"

//empty game object, dont add any super fancy behaviour here
class Empty : public Cappuccino::GameObject {
public:
	Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

};

class MainScene : public Cappuccino::Scene {
public:
	MainScene(bool yn);

	void childUpdate(float dt) override;
	bool init() override;
	bool exit() override;

	void mouseFunction(double xpos, double ypos) override;
private:
	Cappuccino::PointLight _pLight;
	Cappuccino::CappInput _in;

	Cappuccino::Camera _c;
	Empty* _ghoul = nullptr;

};