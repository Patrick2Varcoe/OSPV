#include "terminal.h"
#include "frame_buffer.h"
#include "keyboard.h"

#define INPUT_MAX 128

/* ---------- Utility string functions (no libc) ---------- */

static int str_equal(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == 0 && *b == 0);
}

static int str_len(const char *s)
{
    int n = 0;
    while (s[n]) n++;
    return n;
}

/* ---------- Basic Commands ---------- */

void cmd_help(char *args)
{
    fb_write("Available commands:\n");
    fb_write("  help     - Show this text\n");
    fb_write("  echo     - Print the given text\n");
    fb_write("  clear    - Clear the screen\n");
    fb_write("  version  - Show OS version\n");
}

void cmd_echo(char *args)
{
    fb_write(args);
    fb_write("\n");
}

void cmd_clear(char *args)
{
    fb_clear();
}

void cmd_version(char *args)
{
    fb_write("MyOS v0.1\n");
}

/* ---------- Command Table ---------- */

struct command
{
    const char *name;
    void (*fn)(char *args);
};

static struct command commands[] =
{
    { "help",    cmd_help },
    { "echo",    cmd_echo },
    { "clear",   cmd_clear },
    { "version", cmd_version },
};

#define COMMAND_COUNT (sizeof(commands)/sizeof(commands[0]))

/* ---------- Readline (keyboard-based) ---------- */

char *readline(char *buffer)
{
    int len = 0;

    while (1)
    {
        char c = kb_buffer_pop();
        if (!c) continue;

        // Enter
        if (c == '\n')
        {
            buffer[len] = 0;
            fb_write("\n");
            return buffer;
        }

        // Backspace
        if (c == '\b')
        {
            if (len > 0)
            {
                len--;
                fb_backspace();   // <<< properly handles VGA erasing
            }
            continue;
        }


        // Normal character
        if (len < INPUT_MAX - 1)
        {
            buffer[len++] = c;
            fb_write((char[]){c,0});
        }
    }
}

/* ---------- Parse command ---------- */

void execute_command(char *input)
{
    // Split input into command + args
    char *cmd = input;

    // skip leading spaces
    while (*cmd == ' ') cmd++;

    // find space between command and args
    char *args = cmd;
    while (*args && *args != ' ') args++;

    if (*args)
    {
        *args = 0;   // end command string
        args++;      // start of arguments
    }
    else
    {
        args = "";   // no args
    }

    // Find matching command
    for (unsigned i = 0; i < COMMAND_COUNT; i++)
    {
        if (str_equal(cmd, commands[i].name))
        {
            commands[i].fn(args);
            return;
        }
    }

    fb_write("Unknown command: ");
    fb_write(cmd);
    fb_write("\n");
}

/* ---------- Main terminal loop ---------- */

void terminal_run()
{
    fb_write("myos> ");

    char input[INPUT_MAX];

    while (1)
    {
        readline(input);
        execute_command(input);
        fb_write("myos> ");
    }
}

