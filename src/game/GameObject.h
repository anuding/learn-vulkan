//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_GAMEOBJECT_H
#define ENGINE_GAMEOBJECT_H

#include "Component.h"
#include <vector>
#include <memory>

class GameObject {
public:
	std::vector<std::shared_ptr<Componenet>> components;

	template<typename T>
	std::shared_ptr<T> getComponent() const
	{
		for (auto& component : components) {
			std::shared_ptr<T> actualComp = std::dynamic_pointer_cast<T>(component);
			if (actualComp != nullptr)
				return actualComp;
		}
	};
};


#endif //ENGINE_GAMEOBJECT_H
