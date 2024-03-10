class Plane {
public:
    Plane() {
        timeEntered = 0;
    }

    Plane(unsigned int time) {
        timeEntered = time;
    }

    unsigned int gettimeEntered() {
        return timeEntered;
    }
private:
    unsigned int timeEntered;
};
