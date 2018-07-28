#pragma once

#include <assert.h>
#include <list>

#include "../Library/KitLib/include/Kitlib.h"
#include "CKitBehaviour.h"

namespace kit {
	namespace Engine {

		extern class KitBehaviour;
		extern class AssetsManager g_assetsManager;

		class AssetsManager :public kit::Singleton< AssetsManager > {
		public:
			inline void AddAsset(KitBehaviour* _asset) {
				mlis_assets.emplace_back(_asset);
			}

			bool ClearAssets();

			void Update();
			void Render();
		private:
			friend class kit::Singleton<AssetsManager>;
			AssetsManager();

			std::list<KitBehaviour*> mlis_assets;
		};
	}
}