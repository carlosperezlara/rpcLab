import signal
import time
import curses

import locale
locale.setlocale(locale.LC_ALL, '')
code = locale.getpreferredencoding()


finish = False
def ctrlc(signum, frame):
    print('signal called')

    frame.f_globals['finish'] = True




signal.signal(signal.SIGINT, ctrlc)

from curses import wrapper

def main(stdscr):
    # Clear screen
    stdscr.clear()

    begin_x = 20; begin_y = 7
    height = 5; width = 40
    subwin = stdscr.subwin(0, 50)
    subwin.addstr("Sub win")
    subwin.vline(1,5,'*',25)
    subwin.hline(25,5,'*',50)
    subwin.move(2,2)
    for i in range(2,12):
        subwin.addstr('{0}'.format(i))
        subwin.move(i*2, 2)
    
    stdscr.addstr("Pretty text", curses.color_pair(1))
    stdscr.refresh()
    curses.init_pair(1, curses.COLOR_RED, curses.COLOR_WHITE)
    stdscr.addstr(0,0, "RED ALERT!", curses.color_pair(1))
    stdscr.refresh()
    subwin.refresh()
    stdscr.getkey()

wrapper(main)




# try:
#     while True:
#         time.sleep(1)
#         print('Sleeping Finish? {0}'.format(finish))
#         if finish:
#             break

# except Exception as e:
#     print(e)
# finally:
#     print('Finished with the test')
