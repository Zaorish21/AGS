#include "CRenderManager.h"

int CRenderManager::UBOUpdateCount = 0;

void CRenderManager::init(void)
{
	Shader.LoadVertexShader((char*)"SHADER\\Example.vsh");
	Shader.LoadFragmentShader((char*)"SHADER\\Example.fsh");
	Shader.Link();

	createPerSceneBlock();
}

void CRenderManager::start(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GraphicObjects.clear();
	Shader.Activate();
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
	
	glBindBuffer(GL_UNIFORM_BUFFER, PerSceneUBOIndex);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, PerSceneUBOIndex);
	Shader.SetUniform("tex", 0);
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