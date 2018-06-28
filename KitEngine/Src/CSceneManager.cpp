#include "stdafx.h"

#include "CSceneManager.h"

using namespace kit::Engine;

SceneManager::SceneManager(std::shared_ptr<Scene> _startScene) {
	m_sptrCurrentScene = _startScene;
	m_sptrCurrentScene->SetManagerRef(this);
	m_sptrPreviousScene = nullptr;
}

SceneManager::~SceneManager() {
	g_assetsManager.GetInstance().ClearAssets();
	m_sptrCurrentScene.reset();
	m_sptrPreviousScene.reset();
}

void SceneManager::ChangeScene(const std::shared_ptr<Scene> _changeScene) {
	m_sptrPreviousScene = m_sptrCurrentScene;
	m_sptrCurrentScene = _changeScene;
	m_sptrCurrentScene->SetManagerRef(this);

}

bool SceneManager::UndoChange() {
	if (nullptr != m_sptrPreviousScene) {
		m_sptrCurrentScene = m_sptrPreviousScene;
		m_sptrPreviousScene = m_sptrCurrentScene;
		return true;
	}
	else { return false; }
}

void SceneManager::Update() {
	m_sptrCurrentScene->Update();
}

void SceneManager::Render() {
	m_sptrCurrentScene->Render();
}