# Copied from gitst.github.com/claymcleod -> pycurses.py
import curses

def menu(stdscr):
    k = 0
    cursor_x = 0
    cursor_y = 0

    # Init/set colors
    curses.start_color()
    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_WHITE)

    while (k != ord('q')):

        # Init
        stdscr.clear()
        height, width = stdscr.getmaxyx()

        # Controls from buttons
        if k == curses.KEY_DOWN:
            cursor_y += 1
        elif k == curses.KEY_UP:
            cursor_y -= 1
        elif k == curses.KEY_RIGHT:
            cursor_x += 1
        elif k == curses.KEY_LEFT:
            cursor_x -= 1

        cursor_x = max(0, cursor_x)
        cursor_x = min(width-1, cursor_x)

        cursor_y = max(0, cursor_y)
        cursor_y = min(height-1, cursor_y)

        # Texts
        title = "Curses example"[:width-1]
        subtitle = "At SKA's computer"[:width-1]
        keystr = "Last key pressed: {}".format(k)[:width-1]
        statusbarstr = "Press 'q' to exit | STATUS BAR | Pos: {}, {}".format(
            cursor_x, cursor_y)
        if k == 0:
            keystr = "No key press detected..."[:width-1]

        # Centering calc
        start_x_title = int((width // 2) - (len(title) // 2) - len(title) % 2)
        start_x_subtitle = int(
            (width // 2) - (len(subtitle) // 2) - len(subtitle) % 2)
        start_x_keystr = int(
            (width // 2) - (len(keystr) // 2) - len(keystr) % 2)
        start_y = int((height // 2) - 2)

        # Rendering some txt
        whstr = "Width: {}, Height: {}".format(width, height)
        stdscr.addstr(0, 0, whstr, curses.color_pair(1))

        # Render status bar
        stdscr.attron(curses.color_pair(3))
        stdscr.addstr(height-1, 0, statusbarstr)
        stdscr.addstr(height-1, len(statusbarstr), " " *
                      (width - len(statusbarstr) - 1))
        stdscr.attroff(curses.color_pair(3))

        # Turning on attr for title
        stdscr.attron(curses.color_pair(2))
        stdscr.attron(curses.A_BOLD)

        # Print text
        stdscr.addstr(start_y + 1, start_x_subtitle, subtitle)
        stdscr.addstr(start_y + 3, (width // 2) - 2, "-" * 4)
        stdscr.addstr(start_y + 5, start_x_keystr, keystr)
        stdscr.move(cursor_y, cursor_x)

        # Refresh screen
        stdscr.refresh()

        # Wait for input
        k = stdscr.getch()


def main():
    curses.wrapper(menu)


if __name__ == "__main__":
    main()
