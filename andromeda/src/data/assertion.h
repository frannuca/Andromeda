#pragma once
#define ANDROMEDA_ASSERT(cond, message) if (!###cond) throw "ERROR: message";
