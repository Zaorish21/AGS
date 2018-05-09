#include "CRenderManager.h"

int CRenderManager::UBOUpdateCount = 0;

void CRenderManager::init(void)
{
	Shaders[DIFFUSE_TEXTURE_SHADER_ID].LoadVertexShader((char*)"SHADER\\DirectLight.vsh");
	Shaders[DIFFUSE_TEXTURE_SHADER_ID].LoadFragmentShader((char*)"SHADER\\DirectLight.fsh");
	Shaders[DIFFUSE_TEXTURE_SHADER_ID].Link();

	Shaders[SKYBOX_SHADER_ID].LoadVertexShader((char*)"SHADER\\SkyBox.vsh");
	Shaders[SKYBOX_SHADER_ID].LoadFragmentShader((char*)"SHADER\\SkyBox.fsh");
	Shaders[SKYBOX_SHADER_ID].Link();

	Shaders[AABB_SHADER_ID].LoadVertexShader((char*)"SHADER\\BoundingBox.vsh");
	Shaders[AABB_SHADER_ID].LoadFragmentShader((char*)"SHADER\\BoundingBox.fsh");
	Shaders[AABB_SHADER_ID].Link();

	SkyBoxTextureId = CResourceManager::Instance().LoadCubeTexture("textures/SkyBox/CloudyCrown_Midday/CloudyCrown_Midday");
	SkyBoxMeshId = CResourceManager::Instance().LoadMesh("meshes\\box.obj");
	createPerSceneBlock();
}

void CRenderManager::start(void)
{
	GraphicObjects.clear();
}

void CRenderManager::setCamera(CCamera & Camera)
{
	if (this->Camera != Camera)
	{
		this->Camera = Camera;
		updatePerSceneBlock(PerSceneUBOIndex);
		for (auto it = RenderManagerObjectStates.begin();
			it != RenderManagerObjectStates.end(); it++)
		{
			it->second.UBOIndexUpdate = true;
		};
	};
}

void CRenderManager::setLight(CLight & Light)
{
	if (this->Light != Light)
	{
		this->Light = Light;
		updatePerSceneBlock(PerSceneUBOIndex);
		for (auto it = RenderManagerObjectStates.begin();
			it != RenderManagerObjectStates.end(); it++)
		{
			it->second.UBOIndexUpdate = true;
		};
	};
}

void CRenderManager::addToRenderQueue(CGraphicObject & GraphicObject)
{
	GraphicObjects.push_back(GraphicObject);

	auto it = RenderManagerObjectStates.find(GraphicObject.getID());
	if (it == RenderManagerObjectStates.end())
	{
		GLuint index = createPerObjectBlock(GraphicObject);
		SRenderManagerObjectState state = 
		{
			GraphicObject,
			false,
			index
		};
		RenderManagerObjectStates.insert(std::pair<int, SRenderManagerObjectState>(GraphicObject.getID(),state));
	}
	else if (it->second.GraphicObject != GraphicObject)
	{
		RenderManagerObjectStates[GraphicObject.getID()].GraphicObject = GraphicObject;
		updatePerObjectBlock(RenderManagerObjectStates[GraphicObject.getID()].UBOIndex, GraphicObject);
		RenderManagerObjectStates[GraphicObject.getID()].UBOIndexUpdate = false;
	}
}

void CRenderManager::finish(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	SkyBoxRender();
	
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	
	ObjectsRender();

	if (AABBRenderEnable) AABBRender();
}

int CRenderManager::getObjectCount(void)
{
	return GraphicObjects.size();
}

void CRenderManager::changeAABBRenderMode(void)
{
	AABBRenderEnable = !AABBRenderEnable;
}

void CRenderManager::ObjectsRender()
{
	Shaders[DIFFUSE_TEXTURE_SHADER_ID].Activate();
	Shaders[DIFFUSE_TEXTURE_SHADER_ID].SetUniform("tex", 0);
	glBindBuffer(GL_UNIFORM_BUFFER, PerSceneUBOIndex);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, PerSceneUBOIndex);
	// получаем матрицу проекции
	for (int i = 0; i < GraphicObjects.size(); i++)
	{
		if (RenderManagerObjectStates[GraphicObjects[i].getID()].UBOIndexUpdate)
		{
			updatePerObjectBlock(RenderManagerObjectStates[GraphicObjects[i].getID()].UBOIndex, GraphicObjects[i]);
			RenderManagerObjectStates[GraphicObjects[i].getID()].UBOIndexUpdate = false;
		}
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, RenderManagerObjectStates[GraphicObjects[i].getID()].UBOIndex);

		CTexture* tex = CResourceManager::Instance().GetTexture(GraphicObjects[i].getTexture());
		CMesh* mesh = CResourceManager::Instance().GetMesh(GraphicObjects[i].getMesh());
		if (tex != nullptr) tex->Apply();
		if (mesh != nullptr) mesh->Render();
	}
	CShader::Deactivate();
}

void CRenderManager::SkyBoxRender(void)
{
	Shaders[SKYBOX_SHADER_ID].Activate();
	Shaders[SKYBOX_SHADER_ID].SetUniform("tex", 0);
	Shaders[SKYBOX_SHADER_ID].SetUniformMat4("uProjectionMatrix", Camera.GetProjectionMatrix());
	Shaders[SKYBOX_SHADER_ID].SetUniformMat4("uModelViewMatrix", Camera.GetViewMatrix() * glm::mat4{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
		});
	CTexture* tex = CResourceManager::Instance().GetTexture(SkyBoxTextureId);
	CMesh* mesh = CResourceManager::Instance().GetMesh(SkyBoxMeshId);
	if (tex != nullptr) tex->Apply();
	if (mesh != nullptr) mesh->Render();
}

GLuint CRenderManager::createPerSceneBlock()
{
	glGenBuffers(1, &PerSceneUBOIndex);
	glBindBuffer(GL_UNIFORM_BUFFER, PerSceneUBOIndex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerSceneBlock), nullptr, GL_STATIC_DRAW);
	
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	UBOUpdateCount++;
	return PerSceneUBOIndex;
}

GLuint CRenderManager::createPerObjectBlock(CGraphicObject& GraphicObject)
{
	PerObjectBlock temp =
	{
		Camera.GetViewMatrix()*GraphicObject.getModelMatrix(),
		GraphicObject.getMaterial().getAmbient(),
		GraphicObject.getMaterial().getDiffuse(),
		GraphicObject.getMaterial().getSpecular()
	};

	GLuint tempIndex;
	glGenBuffers(1, &tempIndex);
	glBindBuffer(GL_UNIFORM_BUFFER, tempIndex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerObjectBlock), &temp, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	UBOUpdateCount++;
	return tempIndex;
}

void CRenderManager::updatePerSceneBlock(int UBOindex)
{
	PerSceneBlock temp =
	{
		Camera.GetProjectionMatrix(),
		Light.getAmbient(),
		Light.getDiffuse(),
		Light.getSpecular(),
		Camera.GetViewMatrix()*Light.getPosition()
	};

	glBindBuffer(GL_UNIFORM_BUFFER, UBOindex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerSceneBlock), &temp, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	UBOUpdateCount++;
	return;
}

void CRenderManager::updatePerObjectBlock(int UBOindex, CGraphicObject& GraphicObject)
{
	PerObjectBlock temp =
	{
		Camera.GetViewMatrix()*GraphicObject.getModelMatrix(),
		GraphicObject.getMaterial().getAmbient(),
		GraphicObject.getMaterial().getDiffuse(),
		GraphicObject.getMaterial().getSpecular()
	};

	glBindBuffer(GL_UNIFORM_BUFFER, UBOindex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerObjectBlock), &temp, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	UBOUpdateCount++;
	return;
}

void CRenderManager::AABBRender()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	// активируем шейдер (+устанавливаем некоторые его параметры)
	Shaders[AABB_SHADER_ID].Activate();
	Shaders[AABB_SHADER_ID].SetUniformMat4("ProjectionMatrix", Camera.GetProjectionMatrix());
	Shaders[AABB_SHADER_ID].SetUniform("Color", vec4(1, 0, 0, 1));
	// выводим все модели (их ААВВ)
	for (auto it = GraphicObjects.begin(); it < GraphicObjects.end(); it++) {
		vec3 AABB = it -> getAABB();
		mat4 scale = mat4(
			vec4(AABB.x, 0, 0, 0),
			vec4(0, AABB.y, 0, 0),
			vec4(0, 0, AABB.z, 0),
			vec4(0, 0, 0, 1));
		mat4 ModelViewMatrix = Camera.GetViewMatrix() * it -> getModelMatrix() * scale;
		Shaders[AABB_SHADER_ID].SetUniformMat4("ModelViewMatrix", ModelViewMatrix);
		CMesh* mesh = CResourceManager::Instance().GetMesh(AABBMeshId);
		if (mesh != nullptr) mesh -> Render();
	}
}