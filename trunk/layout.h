#pragma once

#include "frames.h"

#include <vector>

namespace Ash
{
	class HFrame : public Frame
	{
	public:
		virtual bool Layout();
		virtual const char* GetName() { return "HFrame"; }
	};

	class VFrame : public Frame
	{
	public:
		virtual bool Layout();
		virtual const char* GetName() { return "VFrame"; }
	};

}