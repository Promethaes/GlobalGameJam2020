#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappInput.h"
#include "Cappuccino/PointLight.h"
#include "Cappuccino/HitBoxLoader.h"

class HandInteract : public Cappuccino::GameObject {
public:
	HandInteract(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures,unsigned playerNum = 1);

	void childUpdate(float dt) override;
private:
	Cappuccino::CappInput _in;
	unsigned _playerNum = 1;

};

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

	HandInteract* _p1 = nullptr;
	HandInteract* _p2 = nullptr;

	Cappuccino::Camera _c;
	Empty* _table = nullptr;
	Empty* _machineBox = nullptr;

};