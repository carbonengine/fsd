# Copyright © 2023 CCP ehf.

import sys
import blue

sys.modules[__name__] = blue.LoadExtension("_pyfsd")
