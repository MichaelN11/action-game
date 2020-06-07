#pragma once

enum class Keybind { escape, left, right, up, down };

enum class DrawState { none, standLeft, standRight, standUp, standDown, walkLeft, walkRight, walkUp, walkDown };

enum class ActivityState { alive, stunned, dead };

enum class Group { none, player, enemy };