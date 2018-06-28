#include "stdafx.h"

#include "../Inc/CAssetsManager.h"

using namespace kit::Engine;

AssetsManager::AssetsManager() {

}

bool AssetsManager::ClearAssets() {
	mlis_assets.clear();
	assert(0 == mlis_assets.size());
	return true;
}

void AssetsManager::Update() {
	std::list<KitBehaviour*>::iterator iter = mlis_assets.begin();
	while (iter != mlis_assets.end()) {
		KitBehaviour* kb = *iter;

		kb->Update();

		iter++;
	}
}

void AssetsManager::Render() {
	std::list<KitBehaviour*>::iterator iter = mlis_assets.begin();
	while (iter != mlis_assets.end()) {
		KitBehaviour* kb = *iter;

		kb->Render();

		iter++;
	}
}