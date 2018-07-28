#include "stdafx.h"

#include "../Inc/CKitBehaviour.h"

using namespace kit::Engine;

KitBehaviour::KitBehaviour() {
	g_assetsManager.GetInstance().AddAsset(this);
}

KitBehaviour::~KitBehaviour() {

}