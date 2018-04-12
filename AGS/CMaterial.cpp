#include "CMaterial.h"

void CMaterial::setAmbient(vec4 & ambient)
{
	this->ambient = ambient;
}

void CMaterial::setDiffuse(vec4 & diffuse)
{
	this->diffuse = diffuse;
}

void CMaterial::setSpecular(vec4 & specular)
{
	this->specular = specular;
}

void CMaterial::setShininess(float shininess)
{
	this->shininess = shininess;
}

vec4 &CMaterial::getAmbient()
{
	return ambient;
}

vec4 &CMaterial::getDiffuse()
{
	return diffuse;
}

vec4 &CMaterial::getSpecular()
{
	return specular;
}

float &CMaterial::getShininess()
{
	return shininess;
}

bool CMaterial::operator==(CMaterial & a)
{
	return ((ambient == a.ambient) && (diffuse == a.diffuse) && (specular == a.specular) && (shininess == a.shininess));
}

bool CMaterial::operator!=(CMaterial & a)
{
	return ((ambient != a.ambient) || (diffuse != a.diffuse) || (specular != a.specular) || (shininess != a.shininess));
}
