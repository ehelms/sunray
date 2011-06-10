
class Suntracker:
    NUM_DAYS_IN_MONTH = {
        'year': [ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31],
        'leap_year': [ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    }

    def __init__(self):
        self.day_angle_radians = 0
        self.declination_radians = 0
        self.eqt = 0
        self.day = 0
        self.month = 0
        self.year = 0
        self.longitude_direction = 0
        self.latitude_direction = 0
        self.latitude = 0
        self.longitude = 0
        self.altitude = 0
        self.azimuth = 0

        self.solar_flux = 0
        self.latitude_radians = 0
        self.longitude_radians = 0

        self.nrmb_flux = 0

        self.current_TST = 0
        self.tsr = 0
        self.tss = 0
        self.asr = 0
        self.ass = 0

        self.atmtra = 0.65

        self.alt = []
        self.apalt = []
        self.az = []
        self.incident_angle = []
        self.diffuse = []
        self.horb_flux = []
        self.beam = []

    def set_day_angle(self):
        leap = !(self.year % 4) && (self.year % 100) || !(self.year % 400)

        if leap:
            days_in_year = 366
        else:
            days_in_year = 365

        self.day_angle_radians = (2.0 * math.pi * (day - 1)) / days_in_year;
