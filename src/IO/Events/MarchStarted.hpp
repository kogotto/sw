#include <cstdint>

namespace sw::io
{
	struct MarchStarted {
		constexpr static const char* Name = "MARCH_STARTED";

		uint32_t unitId {};
		int x {};
		int y {};
		uint32_t targetX {};
		uint32_t targetY {};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("targetX", targetX);
			visitor.visit("targetY", targetY);
		}
	};
}
