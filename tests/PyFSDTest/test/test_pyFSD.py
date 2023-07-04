import struct
import pyFSD
import unittest

keyOffsetType = struct.Struct("III")
signedKeyOffsetType = struct.Struct("iII")

# For reasons lost to the sands of time there's a glorious hack involving a 32 bit value at the
# start of the underlying FSD data. This hack is leaving its traces all across the codebase,
# including the weird -4 / +4 adjustment in the `FsdUnsignedIntegerKeyMap`.
# Now, when we initially came across this because these tests failed, then we thought the simple
# fix would be to remove the -4 / +4 adjustment from `FsdUnsignedIntegerKeyMap.Initialize()`.
# And while that approach did fix the testsuite for these binaries, then it ended up being wrong
# for the test suite used in the monolith. Turns out, this 32 bit value offset adjustment is done
# in a great many places there, too. Alas, here we are, prepending the test data with a dummy
# 32 bit value to get these tests to pass instead.
monolithFsdOffsetPaddingHack = struct.Struct("I").pack(0)


def IteratorWrapper(pyFSDKeyMap):
    i = pyFSDKeyMap.iteritems()
    while True:
        yield i.next()


class ModuleTests(unittest.TestCase):

    def testSingleEntryLength(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + keyOffsetType.pack(1, 2, 3))

        self.assertEqual(d.length(), 1)

    def testSingleEntryContent(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + keyOffsetType.pack(1, 2, 3))
        self.assertEqual(d.Get(1), (2, 3))

    def testSignedKeyLookup(self):
        '''
        Check that when given and interpreted as unsigned, everything still works
        '''
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + signedKeyOffsetType.pack(-1, 2, 3))
        self.assertEqual(d.Get(-1), (2, 3))

    def testDoubleEntryLength(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        self.assertEqual(d.length(), 2)

    def testDoubleEntryContent(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        self.assertEqual(d.Get(1), (2, 3))
        self.assertEqual(d.Get(4), (5, 6))

    def testMissingElementGet(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        self.assertRaises(IndexError, lambda: d.Get(1))

    def testIteration(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        d.Initialize(monolithFsdOffsetPaddingHack + keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        expected = [(1, (2, 3)), (4, (5, 6))]
        for idx, item in enumerate(IteratorWrapper(d)):
            self.assertEqual(item, expected[idx])

        self.assertEqual(idx, 1)


if __name__ == '__main__':
    unittest.main()
