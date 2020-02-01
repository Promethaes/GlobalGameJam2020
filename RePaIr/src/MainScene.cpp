#include "MainScene.h"
#include "Cappuccino/Events.h"
#include "Cappuccino/Input.h"

MainScene::MainScene(bool yn)
	:Cappuccino::Scene(yn),
	_pLight(glm::vec2(1600.0f, 1200.0f), { /*defaultLight*/glm::vec3(-1.0f,0.0f,5.0f),glm::vec3(-3.0f,0.0f,5.0f) }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
{
	_pLight._pointLightShader.use();

	c.setPosition(glm::vec3(1.0f, 1.0f, 0.0f));
	c.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	_pLight._pointLightShader.loadViewMatrix(c);
	_pLight._pointLightShader.loadProjectionMatrix(1600.0f, 1000.0f);

	_billboardShader.use();
	_billboardShader.loadViewMatrix(c);
	_billboardShader.loadProjectionMatrix(1600.0f, 1000.0f);
	_billboardShader.setUniform("image", 0);
	for (auto x : _pLight.getPositions()) {
		_lightCubes.push_back(new Billboard(&_billboardShader, { new Cappuccino::Texture("defaultNorm.png",Cappuccino::TextureType::DiffuseMap) }));
		_lightCubes.back()->_rigidBody._position = x;
	}

	_box = new Billboard(&_pLight._pointLightShader, { new Cappuccino::Texture("yellow.png",Cappuccino::TextureType::DiffuseMap) });
	_box->_rigidBody._position.z = -1.0f;
	_box->_rigidBody._position.y = 1.0f;
	_box->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.25f);

}

bool MainScene::init()
{
	for (auto x : _lightCubes)
		x->setActive(true);
	_box->setActive(true);

	_initialized = true;
	_shouldExit = false;

	return true;
}

bool MainScene::exit()
{
	_initialized = false;
	_shouldExit = true;

	return true;
}

void MainScene::childUpdate(float dt)
{

	if (isEvent(Events::A))
		c.setPosition(c.getPosition() - glm::vec3(1.0f, 0.0f, 0.f) * dt);

	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(c);
	_billboardShader.use();
	_billboardShader.loadViewMatrix(c);

	static float u = 0.0f;
	static bool reverse = false;

	if (reverse)
		u -= dt;
	else
		u += dt;

	if (u >= 1.0f) {
		u = 1.0f;
		reverse = true;
	}
	else if (u <= 0.0f) {
		u = 0.0f;
		reverse = false;
	}


	_pLight.getPositions()[0] = (glm::vec3(-0.5f * glm::smoothstep(0.0f, 1.0f, u), 1.0f, 5.f * glm::smoothstep(0.0f, 1.0f, u)));
	_lightCubes[0]->_rigidBody._position = _pLight.getPositions()[0];
	_pLight.getPositions()[1] = (glm::vec3(-4.f * glm::smoothstep(1.0f, 0.0f, u), 1.0f, 0.0f));
	_lightCubes[1]->_rigidBody._position = _pLight.getPositions()[1];
	_pLight.resendLights();

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

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	c.doMouseMovement(xOffset, yOffset);
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}
