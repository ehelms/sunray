import suntrackerlib

class Suntracker():
    def __init__(self, lat, lng, year, month, day):
        self.lat = lat
        self.lng = lng
        self.year = year
        self.month = month
        self.day = day
        self.rdayangle = 0
        self.yrlength = 0
        self.julday = 0
    
    def julian(self):
        ret = suntrackerlib.julian(self.year, self.month, self.day, self.julday, self.yrlength, self.rdayangle)
        print ret
        print self.julday
        print self.yrlength
        print self.rdayangle
        return ret
