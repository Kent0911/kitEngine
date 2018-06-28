#pragma once

#include "../Inc/CAssetsManager.h"
#include "../Inc/CScene.h"

namespace kit {
	namespace Engine {
		class SceneManager {
		private:
			std::shared_ptr<Scene> m_sptrCurrentScene;
			std::shared_ptr<Scene> m_sptrPreviousScene;

		public:
			SceneManager(std::shared_ptr<Scene> _startScene);
			~SceneManager();
			void ChangeScene(const std::shared_ptr<Scene> _changeScene);
			bool UndoChange();

			void Update();
			void Render();
		};
	}
}