#include "MainScene.h"
#include "Cappuccino/CappMath.h"

MainScene::MainScene(bool yn)
	:Cappuccino::Scene(yn), _in(true, std::nullopt),
	_pLight(glm::vec2(1600.0f, 1000.0f), { /*defaultLight*/glm::vec3(0.0f,2.0f,1.0f),glm::vec3(0.0f,1.0f,1.0f),glm::vec3(0.0f,1.0f,1.0f) }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 512.0f),
	_mainChain({ Event("getOrder"),Event("grabBeans"),Event("grindBeans") })
{
	_c.setPosition(glm::vec3(-3.168742f, -1.000000f, -6.126548f));
	_c.getFront() = glm::vec3(0.011849f, -0.243615f, -0.969800f);
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(_c);
}


void MainScene::childUpdate(float dt)
{
	//calculate camera movement
	auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::W))
	//	moveForce += glm::vec3(_c.getFront().x, 0.0f, _c.getFront().z);
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::S))
	//	moveForce -= glm::vec3(_c.getFront().x, 0.0f, _c.getFront().z);
	//
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::A))
	//	moveForce -= glm::vec3(_c.getRight().x, 0.0f, _c.getRight().z);
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::D))
	//	moveForce += glm::vec3(_c.getRight().x, 0.0f, _c.getRight().z);
	//
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::SPACE))
	//	moveForce += glm::vec3(0.0f, 1.0f, 0.0f);
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_SHIFT))
	//	moveForce -= glm::vec3(0.0f, 1.0f, 0.0f);



	float speed = 1.5f;
	moveForce *= speed;



	_c.setPosition(_c.getPosition() + moveForce * dt);

	//load view matrix
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(_c);


	static float elapsedTime = 0.0f;
	elapsedTime += dt;

	static float u = 0.0f;
	static bool reverse = false;
	static float scalar = 0.0f;

	if (reverse)
		u -= dt * 1.5f;
	else
		u += dt * 1.5f;

	if (u >= 1.0f) {
		u = 1.0f;
		reverse = true;
	}
	else if (u <= 0.0f) {
		u = 0.0f;
		reverse = false;
	}
	scalar = 1.0f + 0.01f * glm::smoothstep(0.0f, 1.f, u);
	_pLight._pointLightShader.setUniform("scalar", scalar);
	//for (auto x : Cappuccino::GameObject::gameObjects) {
	//	x->_transform._scaleMat[0].x = offset;
	//	x->_transform._scaleMat[1].y = offset;
	//	x->_transform._scaleMat[2].z = offset;
	//}

	//_ghoul->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90*dt);

	//printf("%f,%f,%f\n", _p1->_rigidBody._position.x, _p1->_rigidBody._position.y, _p1->_rigidBody._position.z);
	if (!_mainChain._events[0]._completed) {
		if(!_grinder->isTriggered() && _in.keyboard->keyPressed('E'))
		_grinder->setTrigger(true);
		
		if (_grinder->_machineTimer >= 2.0f) {
			_grinder->_machineTimer = 0.0f;
			_mainChain._events[0]._completed = true;
		}

	}
	else if (_mainChain._events[0]._completed && !_mainChain._events[1]._completed) {

	}//and so on...

}

bool MainScene::init()
{

	//set the cursor so that its locked to the window
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize the ghoul and the bullet
	if (_table == nullptr) {
		_table = new Empty(_pLight._pointLightShader, { new Cappuccino::Texture("yellow.png",Cappuccino::TextureType::DiffuseMap),
			new Cappuccino::Texture("yellow.png",Cappuccino::TextureType::SpecularMap) }, { new Cappuccino::Mesh("desk.obj") });
		_table->_rigidBody._position = -glm::vec3(3.0f, 3.0f, 10.0f);
	}
	if (_machineBox == nullptr) {
		_machineBox = new Empty(_pLight._pointLightShader, { new Cappuccino::Texture("yellow.png",Cappuccino::TextureType::DiffuseMap),
			new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::SpecularMap) }, { new Cappuccino::Mesh("testSquare.obj") });
		_machineBox->_rigidBody._position = _table->_rigidBody._position;
	}
	if (_p1 == nullptr) {
		_p1 = new HandInteract(_pLight._pointLightShader, { new Cappuccino::Texture("white.png",Cappuccino::TextureType::DiffuseMap),
			new Cappuccino::Texture("white.png",Cappuccino::TextureType::SpecularMap),new Cappuccino::Texture("white.png",Cappuccino::TextureType::EmissionMap) });
		_p1->_rigidBody._position = _table->_rigidBody._position;
		_p1->_rigidBody._position.y += 1.0f;
		_p1->_rigidBody._position.x -= 1.0f;
	}
	if (_p2 == nullptr) {
		_p2 = new HandInteract(_pLight._pointLightShader, { new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::DiffuseMap),
			new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::SpecularMap),new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::EmissionMap) }, 2);
		_p2->_rigidBody._position = _table->_rigidBody._position;
		_p2->_rigidBody._position.y += 1.0f;
		_p2->_rigidBody._position.x += 1.f;
	}
	if (_grinder == nullptr) {
		_grinder = new Grinder(_pLight._pointLightShader, { new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::DiffuseMap),
			new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::SpecularMap),new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::EmissionMap) },
			{ new Cappuccino::Mesh("BeanGrindBox.obj") },Cappuccino::HitBox(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f)));
		_grinder->_rigidBody._position = _p1->_rigidBody._position;
		_grinder->_rigidBody._position.x += 0.5f;
		_grinder->_rigidBody._position.y -= 1.0f;
	}


	_table->setActive(true);
	_machineBox->setActive(true);
	_p1->setActive(true);
	_p2->setActive(true);
	_grinder->setActive(true);

	_initialized = true;
	_shouldExit = false;
	return true;
}

bool MainScene::exit()
{
	_table->setActive(false);

	_initialized = false;
	_shouldExit = true;
	return false;
}

void MainScene::mouseFunction(double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = 400, lastY = 300;
	static float yaw = -90.0f;
	static float pitch = 0.0f;
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	//_c.doMouseMovement(xOffset, yOffset);
}

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Cappuccino::GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
}

HandInteract::HandInteract(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, unsigned playerNum)
	:GameObject(SHADER, textures, { new Cappuccino::Mesh("testSquare.obj") }), _in(true, std::nullopt)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/testSquare.obj");
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);
	_rigidBody.setGrav(false);
	_playerNum = playerNum;
}

void HandInteract::childUpdate(float dt)
{
	auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
	if (_playerNum == 1) {
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::W))
			moveForce += glm::vec3(0.0f, 1.0f, 0.0f);
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::S))
			moveForce -= glm::vec3(0.0f, 1.0f, 0.0f);

		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::A))
			moveForce -= glm::vec3(1.0f, 0.0f, 0.0f);
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::D))
			moveForce += glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (_playerNum == 2) {
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::UP_ARROW))
			moveForce += glm::vec3(0.0f, 1.0f, 0.0f);
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::DOWN_ARROW))
			moveForce -= glm::vec3(0.0f, 1.0f, 0.0f);

		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_ARROW))
			moveForce -= glm::vec3(1.0f, 0.0f, 0.0f);
		if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::RIGHT_ARROW))
			moveForce += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	//-3.437498,-2.175001,-10.000000 left
	if (_playerNum == 1 && _rigidBody._position.x >= -3.43f)
		_rigidBody._position.x = -3.43f;
	//-2.499994,-2.175001,-10.000000 right
	if (_playerNum == 2 && _rigidBody._position.x <= -2.49f)
		_rigidBody._position.x = -2.49f;
	if (_rigidBody._position.y <= -3.0f)
		_rigidBody._position.y = -3.0f;


	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::SPACE))
	//	moveForce += glm::vec3(0.0f, 1.0f, 0.0f);
	//if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_SHIFT))
	//	moveForce -= glm::vec3(0.0f, 1.0f, 0.0f);



	float speed = 1.5f;
	moveForce *= speed;
	_rigidBody.setVelocity(moveForce);
}

MachineInteract::MachineInteract(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const Cappuccino::HitBox& triggerVolume)
	:GameObject(SHADER, textures, meshes), _triggerVolume(triggerVolume)
{
	_rigidBody.setGrav(false);
}

void MachineInteract::childUpdate(float dt)
{
	if (_isTriggered)
		turnOn(dt);
}

Grinder::Grinder(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const Cappuccino::HitBox& triggerVolume)
	:MachineInteract(SHADER,textures,meshes,triggerVolume),_onSound("machineSound.wav","Grinder")
{

}

void Grinder::turnOn(float dt)
{
	if (!_playedSound) {
		_onSound.play();
		_playedSound = true;
	}
	_machineTimer += dt;

}
