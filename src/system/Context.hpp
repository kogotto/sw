#pragma once

namespace sw {
class Units;
class EventLog;
} // namespace sw

namespace sw {

class Context {
public:
    Context(sw::Units& units, sw::EventLog& eventLog)
        : _units{units}
        , _eventLog{eventLog}
    {}

    int getTickNumber() const { return _tickNumber; }
    void setTickNumber(int newNumber) { _tickNumber = newNumber; }

    sw::Units& units() { return _units; }
    const sw::Units& units() const { return _units; }

    sw::EventLog& eventLog() { return _eventLog; }
    const sw::EventLog& eventLog() const { return _eventLog; }

    template <typename TEvent>
    void log(TEvent&& event) {
        eventLog().log(
            getTickNumber(),
            event
        );
    }
private:
    int _tickNumber{0};
    sw::Units& _units;
    sw::EventLog& _eventLog;
};

} // namespace sw
