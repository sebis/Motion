#ifndef COMMON_COLLISIONBODY
#define COMMON_COLLISIONBODY

namespace Common
{
	class CollisionBody
	{
	public:
		enum Type {
			RIGID,
			SOFT,
		};

		CollisionBody(Type type = RIGID) : m_type(type) {}
		virtual ~CollisionBody() {}
	
		inline Type type() const { return m_type; }

	private:
		Type m_type;
	};
}

#endif /* COMMON_COLLISIONBODY */