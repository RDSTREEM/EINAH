#include "utils.h"

bool isSkippable(char c)
{
    return c == ' ' || c == '\n' || c == '\t'; // Check if character is skippable
}