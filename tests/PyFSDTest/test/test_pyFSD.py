import struct
import pyFSD
import unittest

keyOffsetType = struct.Struct("III")
signedKeyOffsetType = struct.Struct("iII")


class ModuleTests(unittest.TestCase):

    def testSingleEntryLength(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap(keyOffsetType.pack(1, 2, 3))

        self.assertEqual(d.length(), 1)

    def testSingleEntryContent(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap(keyOffsetType.pack(1, 2, 3))
        self.assertEqual(d.Get(1), (2, 3))

    def testSignedKeyLookup(self):
        '''
        Check that when given and interpreted as unsigned, everything still works
        '''
        d = pyFSD.FsdUnsignedIntegerKeyMap(signedKeyOffsetType.pack(-1, 2, 3))
        self.assertEqual(d.Get(-1), (2, 3))

    def testDoubleEntryLength(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap(keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        self.assertEqual(d.length(), 2)

    def testDoubleEntryContent(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap(keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        self.assertEqual(d.Get(1), (2, 3))
        self.assertEqual(d.Get(4), (5, 6))

    def testMissingElementGet(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap()
        self.assertRaises(IndexError, lambda: d.Get(1))

    def testIteration(self):
        d = pyFSD.FsdUnsignedIntegerKeyMap(keyOffsetType.pack(1, 2, 3) + keyOffsetType.pack(4, 5, 6))

        expected = [(1, (2, 3)), (4, (5, 6))]
        for idx, item in enumerate(d):
            self.assertEqual(item, expected[idx])

        self.assertEqual(idx, 1)


if __name__ == '__main__':
    unittest.main()
