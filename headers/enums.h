#pragma once

enum class Direction { left, right, up, down, none };

enum class Keybind { escape, left, right, up, down, attack };

enum class DrawState { none, standLeft, standRight, standUp, standDown, walkLeft, walkRight, walkUp, walkDown, attackLeft, attackRight, attackUp, attackDown, stunned, dead };

enum class ActivityState { alive, stunned, dead };

enum class Group { none, player, enemy };

enum class Behavior { none, wander };