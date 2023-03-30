from os import fork, wait, WNOHANG, kill, waitpid
from signal import SIGINT, SIGTERM
from time import sleep
from typing import Callable

def child():
    print("Child process")
    while True:
        print("Child process running")
        sleep(1)
    print("Child process done")

def timed_child(seconds,text="NO TEXT"):
    sleep(seconds)
    print(text)


class MultiProcessManager:
    children = []

    def child_wrapper(self, fn : Callable, argvs, kargvs):
        try:
            fn(*argvs, **kargvs)
        except Exception as e:
            print("Child process recived exception :", e)
        finally:
            exit(0)

    def spawn_process(self, fn : Callable, *argvs, **kargvs):
        pid = fork()
        if pid == 0:
            self.child_wrapper(fn, argvs, kargvs)
        else:
            self.children.append(pid)

    def loop_children(self):
        while len(self.children):
                pid, exit_status = wait()
                print(f"Process pid {pid} exited with status {exit_status}")
                self.children.remove(pid)

    def force_children_exit(self):
        for pid in self.children:
            kill(pid, SIGTERM)
        for pid in self.children:
            waitpid(pid, WNOHANG)
            print(f"Process pid {pid} forced to exit")

    def main(self):
        try:
            self.loop_children()
        except KeyboardInterrupt:
            print("Keyboard Interrupt")
        except Exception as e:
            print(f"Unexpected excpetion {e}")
        finally:
            print("Exiting program")
            self.force_children_exit()
            exit(0)
        


manager = MultiProcessManager()
manager.spawn_process(child)
manager.spawn_process(timed_child, 3,text="CIAO BELLO")
manager.main()