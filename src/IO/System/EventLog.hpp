#pragma once

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <type_traits>

#include "IO/System/details/PrintFieldVisitor.hpp"

namespace sw
{
	class EventLog {
	public:
		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			std::cout << "[" << tick << "] " << std::decay_t<TEvent>::Name << " ";
			PrintFieldVisitor visitor(std::cout);
			event.visit(visitor);
			std::cout << std::endl;
		}
	};
}
