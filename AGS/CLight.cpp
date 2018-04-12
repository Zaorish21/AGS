#include "CLight.h"

void CLight::setPosition(vec4 & position)
{
	this->position = position;
}

void CLight::setAmbient(vec4 & ambient)
{
	this->ambient = ambient;
}

void CLight::setDiffuse(vec4 & diffuse)
{
	this->diffuse = diffuse;
}

void CLight::setSpecular(vec4 & specular)
{
	this->specular = specular;
}

vec4 &CLight::getPosition()
{
	return position;
}

vec4 &CLight::getAmbient()
{
	return ambient;
}

vec4 &CLight::getDiffuse()
{
	return diffuse;
}

vec4 &CLight::getSpecular()
{
	return specular;
}

bool CLight::operator==(CLight & a)
{
	return ((position == a.position)&&(ambient == a.position)&&(diffuse==a.diffuse)&&(specular==a.specular));
}

bool CLight::operator!=(CLight & a)
{
	return ((position != a.position) || (ambient != a.ambient) || (diffuse != a.diffuse) || (specular != a.specular));
}
