#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/GameObject.h"
#include "Cappuccino/CappInput.h"
#include "Cappuccino/PointLight.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"

#include "EventChain.h"

class MachineInteract : public Cappuccino::GameObject {
public:
	MachineInteract(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes,const Cappuccino::HitBox& triggerVolume);

	void childUpdate(float dt) override;

	Cappuccino::HitBox _triggerVolume;

	bool isTriggered() { return _isTriggered; }
	void  setTrigger(bool yn) { _isTriggered = yn; }

	float _machineTimer = 0.0f;
protected:
	//return true when done
	virtual void turnOn(float dt) = 0;
private:
	bool _isTriggered = false;
};


class Grinder : public MachineInteract {
public:
	Grinder(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const Cappuccino::HitBox& triggerVolume);

	Cappuccino::Sound _onSound;

	void turnOn(float dt) override;
	bool _playedSound = false;
};


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
	Grinder* _grinder = nullptr;

	EventChain _mainChain;
	Cappuccino::PointLight _pLight;
	Cappuccino::CappInput _in;

	HandInteract* _p1 = nullptr;
	HandInteract* _p2 = nullptr;

	Cappuccino::Camera _c;
	Empty* _table = nullptr;
	Empty* _machineBox = nullptr;

};