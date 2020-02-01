#include "Billboard.h"

Billboard::Billboard(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures)
	:Cappuccino::GameObject(*SHADER, textures, { new Cappuccino::Mesh("rectangle_test.obj") })
{
	_rigidBody.setGrav(false);
}

void Billboard::childUpdate(float dt)
{
}
