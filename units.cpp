#include "units.h"

Units::Units()
{
}

double Units::convertSpeed(VelocityUnit from, VelocityUnit to, double value) {
    double ms = 0;
    if(from == VELOCITY_KTS)
        ms = value * 0.51;
    if(from == VELOCITY_KMH)
        ms = value * 0.278;
    if(from == VELOCITY_MS)
        ms = value;
    if(from == VELOCITY_FPM)
        ms = value * 0.00508;

    if(to == VELOCITY_KMH)
        return ms * 3.6;
    if(to == VELOCITY_KTS)
        return ms * 1.94;
    if(to == VELOCITY_FPM)
        return ms * 196.850393;
    return ms;
}

double Units::convertDistance(DistanceUnit from, DistanceUnit to, double value) {
    double m = 0;
    if(from == DISTANCE_M)
        m = value;
    if(from == DISTANCE_FT)
        m = value * 0.3048;

    if(to == DISTANCE_FT)
        return m*3.2808399;

    return m;
}

QString Units::unitName(VelocityUnit unit) {
    if(unit == VELOCITY_KTS)
        return "kts";
    if(unit == VELOCITY_KMH)
        return "km/h";
    if(unit == VELOCITY_MS)
        return "m/s";
    if(unit == VELOCITY_FPM)
        return "ft/min";
    return "?";
}

QString Units::unitName(DistanceUnit unit) {
    if(unit == DISTANCE_M)
        return "meters";
    if(unit == DISTANCE_FT)
        return "feet";
    return "?";
}

VelocityUnit Units::velocityUnitForName(QString name) {
    if(name=="kts") return VELOCITY_KTS;
    if(name=="km/h") return VELOCITY_KMH;
    if(name=="m/s") return VELOCITY_MS;
    if(name=="ft/min") return VELOCITY_FPM;
    return VELOCITY_UNKNOWN;
}

DistanceUnit Units::distanceUnitForName(QString name) {
    if(name=="meters") return DISTANCE_M;
    if(name=="feet") return DISTANCE_FT;
    return DISTANCE_UNKNOWN;
}
