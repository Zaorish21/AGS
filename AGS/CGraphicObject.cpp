#include "CGraphicObject.h"

void CGraphicObject::setID(int ID)
{
	this->ID = ID;
}

void CGraphicObject::setMesh(int meshID)
{
	this->meshID = meshID;
}

void CGraphicObject::setTexture(int textureID)
{
	this->textureID = textureID;
}

void CGraphicObject::setMaterial(CMaterial & material)
{
	this->material = material;
}

void CGraphicObject::setPosition(vec3 position)
{
	modelMatrix = mat4(
		vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		vec4(position, 1));
}

void CGraphicObject::setRotation(float degree)
{
	degree = radians(degree);
	mat4 Rotate = mat4
	(vec4(cos(degree), 0.0, sin(degree), 0),
		vec4(0.0, 1.0, 0.0, 0),
		vec4(-sin(degree), 0.0, cos(degree), 0),
		vec4(0, 0, 0, 1));
	this->modelMatrix = modelMatrix * Rotate;
}

void CGraphicObject::setAABB(vec3 AABB)
{
	this->AABB = AABB;
}

int CGraphicObject::getMesh(void)
{
	return meshID;
}

int CGraphicObject::getTexture(void)
{
	return textureID;
}

int CGraphicObject::getID(void)
{
	return ID;
}

CMaterial & CGraphicObject::getMaterial(void)
{
	// TODO: вставьте здесь оператор return
	return material;
}

mat4 CGraphicObject::getModelMatrix(void)
{
	return modelMatrix;
}

vec3 CGraphicObject::getAABB()
{
	return AABB;
}

bool CGraphicObject::operator==(CGraphicObject & a)
{
	return ((ID == a.ID) && (meshID == a.meshID) && (material == a.material) && (modelMatrix == a.modelMatrix));
}

bool CGraphicObject::operator!=(CGraphicObject & a)
{
	return ((ID != a.ID) || (meshID != a.meshID) || (material != a.material) || (modelMatrix != a.modelMatrix));;
}
