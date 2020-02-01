#include "MainScene.h"

MainScene::MainScene(bool yn)
	:Cappuccino::Scene(yn), _in(true, std::nullopt),
	_pLight(glm::vec2(1600.0f, 1000.0f), { /*defaultLight*/glm::vec3(-3.0f,0.0f,5.0f),glm::vec3(-3.0f,0.0f,5.0f) }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 512.0f)
{
	_c.setPosition(_c.getPosition() + glm::vec3(0.0f, 0.0f, 0.0f));
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(_c);
}


void MainScene::childUpdate(float dt)
{
	//calculate camera movement
	auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
	if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::W))
		moveForce += glm::vec3(_c.getFront().x, 0.0f, _c.getFront().z);
	if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::S))
		moveForce -= glm::vec3(_c.getFront().x, 0.0f, _c.getFront().z);

	if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::A))
		moveForce -= glm::vec3(_c.getRight().x, 0.0f, _c.getRight().z);
	if (_in.keyboard->keyPressed(Cappuccino::KeyEvent::D))
		moveForce += glm::vec3(_c.getRight().x, 0.0f, _c.getRight().z);

	float speed = 1.5f;
	moveForce *= speed;

	_c.setPosition(_c.getPosition() + moveForce * dt);

	//load view matrix
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(_c);


	static float elapsedTime = 0.0f;
	elapsedTime += dt;

	_ghoul->_rigidBody._position.y = sinf(elapsedTime);


}

bool MainScene::init()
{

	//set the cursor so that its locked to the window
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize the ghoul and the bullet
	if (_ghoul == nullptr) {
		_ghoul = new Empty(_pLight._pointLightShader, {new Cappuccino::Texture("yellow.png",Cappuccino::TextureType::DiffuseMap)}, { new Cappuccino::Mesh("rectangle_test.obj") });
		_ghoul->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
	}

	_ghoul->setActive(true);

	_initialized = true;
	_shouldExit = false;
	return true;
}

bool MainScene::exit()
{
	_ghoul->setActive(false);

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

	_c.doMouseMovement(xOffset, yOffset);
}

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Cappuccino::GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
//	_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), dt * 90.0f);
}
