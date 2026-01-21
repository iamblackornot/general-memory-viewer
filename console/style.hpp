#pragma once

#define COLOR_RESET   "\033[0m"

#define BG_BLACK       "\033[40m"
#define BG_RED         "\033[41m"
#define BG_GREEN       "\033[42m"
#define BG_YELLOW      "\033[43m"
#define BG_BLUE        "\033[44m"
#define BG_MAGENTA     "\033[45m"
#define BG_CYAN        "\033[46m"
#define BG_WHITE       "\033[47m"

#define FG_BLACK       "\033[30m"
#define FG_RED         "\033[31m"
#define FG_GREEN       "\033[32m"
#define FG_YELLOW      "\033[33m"
#define FG_BLUE        "\033[34m"
#define FG_MAGENTA     "\033[35m"
#define FG_CYAN        "\033[36m"
#define FG_WHITE       "\033[37m"

#define BG_BRIGHT_BLACK   "\033[100m"
#define BG_BRIGHT_RED     "\033[101m"
#define BG_BRIGHT_GREEN   "\033[102m"
#define BG_BRIGHT_YELLOW  "\033[103m"
#define BG_BRIGHT_BLUE    "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN    "\033[106m"
#define BG_BRIGHT_WHITE   "\033[107m"

#define FG_BRIGHT_BLACK   "\033[90m"
#define FG_BRIGHT_RED     "\033[91m"
#define FG_BRIGHT_GREEN   "\033[92m"
#define FG_BRIGHT_YELLOW  "\033[93m"
#define FG_BRIGHT_BLUE    "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN    "\033[96m"
#define FG_BRIGHT_WHITE   "\033[97m"

#define STYLE_BOLD      "\033[1m"
#define STYLE_DIM       "\033[2m"
#define STYLE_UNDERLINE "\033[4m"
#define STYLE_REVERSE   "\033[7m"
#define STYLE_HIDDEN    "\033[8m"

#define BG_COLOR_256(n) "\033[48;5;" #n "m"
#define FG_COLOR_256(n) "\033[38;5;" #n "m"

#define BG_RED_256         "\033[48;5;196m"      // Bright red
#define BG_NAVY_BLUE_256   "\033[48;5;18m"       // Dark blue
#define BG_GREEN_256       "\033[48;5;28m"       // Dark green
#define BG_BLACK_256       "\033[48;5;0m"        // Black
#define BG_TEAL_256        "\033[48;5;30m"       // Dark cyan
#define BG_CYAN_256        "\033[48;5;51m"       // Bright cyan
#define BG_PURPLE_256      "\033[48;5;91m"       // Dark magenta
#define BG_YELLOW_256      "\033[48;5;226m"      // Bright yellow
#define BG_OLIVE_256       "\033[48;5;100m"      // Olive
#define BG_GRAY_256        "\033[48;5;245m"      // Medium gray
#define BG_SKY_BLUE_256    "\033[48;5;39m"       // Light blue
#define BG_PINK_256        "\033[48;5;212m"      // Light magenta
#define BG_LIGHT_GREEN_256 "\033[48;5;120m"      // Light green
#define BG_LAVENDER_256    "\033[48;5;147m"      // Light purple
#define BG_ORANGE_256      "\033[48;5;208m"      // Orange
#define BG_OLIVE_GREEN_256 "\033[48;5;64m"       // Dark olive
#define BG_LIGHT_GRAY_256  "\033[48;5;250m"      // Light gray
#define BG_SALMON_256      "\033[48;5;210m"      // Light red
#define BG_CREAM_256       "\033[48;5;230m"      // Very light yellow
#define BG_INDIGO_256      "\033[48;5;55m"       // Dark purple