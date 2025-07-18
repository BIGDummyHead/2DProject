//
// Created by shawn on 7/18/2025.
//

#ifndef TIMESOUND_H
#define TIMESOUND_H

struct TimeSound {

private:
    long long milliseconds;
    int seconds;
    int minutes;
    int hours;
    int days;

public:

    explicit TimeSound(long long totalMS) {

        if(totalMS < 0) {
            totalMS = 0;
        }

        milliseconds = totalMS;
        seconds = milliseconds / 1000;
        minutes = seconds / 60;
        hours = minutes / 60;
        days = hours / 24;
    }

    TimeSound(const TimeSound& other) : TimeSound(other.milliseconds) {

    }

    static TimeSound createFromMilliSeconds(const long long ms) {
        return TimeSound(ms);
    }

    static TimeSound createFromSeconds(const int seconds) {
        return TimeSound(seconds * 1000);
    }

    static TimeSound createFromMinutes(const int minutes) {
        return  createFromSeconds(minutes * 60);
    }

    static TimeSound createFromHours(const int hours) {
        return createFromMinutes(hours * 60);
    }

    static TimeSound createFromDays(const int days) {
        return createFromHours(days * 24);
    }

    [[nodiscard]] long long getMilliseconds() const {
        return milliseconds;
    }

    [[nodiscard]] int getSeconds() const {
        return seconds;
    }

    [[nodiscard]] int getMinutes() const {
        return minutes;
    }

    [[nodiscard]]int getHours() const {
        return hours;
    }

    [[nodiscard]]int getDays() const {
        return days;
    }

    bool operator==(const TimeSound& other) const {
        return getMilliseconds() == other.getMilliseconds();
    }

    bool operator !=(const TimeSound& other) const {
        return !(*this == other);
    }

    bool operator<(const TimeSound& other) const {
        return getMilliseconds() < other.getMilliseconds();
    }

    bool operator>(const TimeSound& other) const {
        return  getMilliseconds() > other.getMilliseconds();
    }

    bool operator<=(const TimeSound& other) const {
        return *this == other || *this < other;
    }

    bool operator>=(const TimeSound& other) const {
        return *this == other || *this > other;
    }

    TimeSound operator+(const TimeSound& other) const {
        const auto msDif = getMilliseconds() + other.getMilliseconds();
        return TimeSound(msDif);
    }

    TimeSound operator-(const TimeSound& other) const {
        const auto msDif = getMilliseconds() - other.getMilliseconds();
        return TimeSound(msDif);
    }

};

#endif //TIMESOUND_H
