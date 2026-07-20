#pragma once

typedef struct
{
	void (*init)(void);
	void (*handle_input)(void);
	void (*handle_update)(void);
} GameTableEntry;
