#pragma once

enum struct eUnplugAction: uint8_t
{
	Unspecified = 0,
	Message = 1,
	Sleep = 2,
	Hibernate = 3,
};