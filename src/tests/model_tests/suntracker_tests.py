import unittest

from sunray.model.suntracker import Suntracker

class SuntrackerTest(unittest.TestCase):
    def setUp(self):
        self.lat = -70
        self.lng = -78
        self.year = 2011
        self.month = 3
        self.day = 20
        self.suntracker = Suntracker(self.lat, self.lng,
                                    self.year, self.month,
                                    self.day)

    def test_julian(self):
        ret = self.suntracker.julian()
        print ret
        self.assertTrue(True)
