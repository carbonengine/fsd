# Copyright © 2023 CCP ehf.

try:
    from _pyfsd_debug import *
except ImportError:
    pass

try:
    from _pyfsd_internal import *
except ImportError:
    pass

try:
    from _pyfsd_trinitydev import *
except ImportError:
    pass

try:
    from _pyfsd import *
except ImportError:
    pass