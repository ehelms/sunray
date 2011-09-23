import unittest
import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)) + '/../' )

from tests.model_tests.suntracker_tests import SuntrackerTest


if __name__ == '__main__':
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(SuntrackerTest))
    unittest.TextTestRunner(verbosity=2).run(suite)
