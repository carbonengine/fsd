import sys
import unittest


sys.modules["pyFSD"] = sys


def print_suite(suite):
    if hasattr(suite, "_exception"):
        print(suite._exception)
        sys.exit(1)
    elif not hasattr(suite, "__iter__"):
        print(suite.id())
    else:
        for item in suite:
            print_suite(item)


if __name__ == "__main__":
    suite = unittest.defaultTestLoader.discover(".")
    print_suite(suite)
