#pragma once

enum struct eUnplugAction: uint8_t
{
	Unspecified = 0,
	Message = 1,
	Sleep = 2,
	Hibernate = 3,
};

class UnplugAction
{
	uint8_t packed;

	UnplugAction( DWORD loaded ) noexcept;

public:
	UnplugAction() noexcept : packed( 0 ) { }
	UnplugAction( eUnplugAction act, bool enableWakeupEvents ) noexcept;
	UnplugAction( const UnplugAction& ) noexcept = default;

	// True if the class is empty
	bool empty() const noexcept
	{
		return 0 == packed;
	}

	// Action to take when AC power is disconnected
	eUnplugAction action() const noexcept
	{
		return (eUnplugAction)( packed & 0x0F );
	}

	// bWakeupEventsDisabled parameter for SetSuspendState API
	BOOL wakeupEventsDisabled() const noexcept
	{
		return ( 0 != ( packed & 0x80 ) ) ? FALSE : TRUE;
	}

	// Store to registry
	HRESULT store() const noexcept;

	// Try to load the preferences from registry.
	// Returns an empty value if missing, or unable to load for any other reason.
	static UnplugAction load() noexcept;
};