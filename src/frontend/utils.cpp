#include "utils.h"

bool isSkippable(char c)
{
    return c == ' ' || c == '\n' || c == '\t'; // Space, newline, or tab
}