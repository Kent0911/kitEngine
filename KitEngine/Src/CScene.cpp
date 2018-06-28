#include "stdafx.h"

#include "../Inc/CScene.h"

using namespace kit::Engine;

void Scene::SetManagerRef(SceneManager* _ref) {
	mc_sceneManager = _ref;
}

void Scene::Update() {
	g_assetsManager.GetInstance().Update();
}

void Scene::Render() {
	g_assetsManager.GetInstance().Render();
}