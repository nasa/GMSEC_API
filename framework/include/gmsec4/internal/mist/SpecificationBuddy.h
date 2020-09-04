#ifndef GMSEC_API_INTERNAL_SPECIFICATION_BUDDY_H
#define GMSEC_API_INTERNAL_SPECIFICATION_BUDDY_H


#include <gmsec4/mist/Specification.h>
#include <gmsec4/internal/mist/InternalSpecification.h>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class SpecificationBuddy
{
public:
	static InternalSpecification& getInternal(const Specification& spec)
	{
		return *spec.m_internal;
	}
};


} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec


#endif
