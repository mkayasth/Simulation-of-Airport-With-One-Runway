#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include "queueAsArray.h"
#include "class.h" // includes the Plane class.

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    // User inputs.
    unsigned int simulationTime;
    int landingTime, takeoffTime, maxFuel, avgTimeBetweenLandings, avgTimeBetweenTakeoffs;

    cout << "Enter the time required for one plane to land (minutes): ";
    cin >> landingTime;
    cout << "Enter the time required for one plane to take off (minutes): ";
    cin >> takeoffTime;
    cout << "Enter the average amount of time between the arrival of planes to the landing queue (minutes): ";
    cin >> avgTimeBetweenLandings;
    cout << "Enter the average amount of time between the arrival of planes to the takeoff queue (minutes): ";
    cin >> avgTimeBetweenTakeoffs;
    cout << "Enter the maximum time a plane can stay in the landing queue without running out of fuel (minutes): ";
    cin >> maxFuel;
    cout << "Enter the total time to simulate (minutes): ";
    cin >> simulationTime;

    // manually specified probability. Variability for takeoffs and landings set to 50%. This is to account for unexpected delays.
    double landingVariability = avgTimeBetweenLandings * 0.5;
    double takeoffVariability = avgTimeBetweenTakeoffs * 0.5;

    queueType<Plane> landingQueue;
    queueType<Plane> takeoffQueue;

    unsigned int currentTime = 0;

    // output variables.
    unsigned int planesLanded = 0, planesTookOff = 0, planesCrashed = 0;
    unsigned int totalLandingWait = 0, totalTakeoffWait = 0;

    unsigned int nextLandingTime = currentTime + avgTimeBetweenLandings;
    unsigned int nextTakeoffTime = currentTime + avgTimeBetweenTakeoffs;

    while (currentTime < simulationTime) {
        // Check for new arrivals
        if (currentTime >= nextLandingTime) {
            landingQueue.addQueue(Plane(currentTime));
            nextLandingTime = currentTime + avgTimeBetweenLandings - landingVariability + (rand() % static_cast<int>(2 * landingVariability)); // sets variability to 50%.
        }
        if (currentTime >= nextTakeoffTime) {
            takeoffQueue.addQueue(Plane(currentTime));
            nextTakeoffTime = currentTime + avgTimeBetweenTakeoffs - takeoffVariability + (rand() % static_cast<int>(2 * takeoffVariability)); // sets variability to 50%.
        }

        // Process landing queue first
        if (!landingQueue.isEmptyQueue()) {
            Plane plane = landingQueue.front();

            if (currentTime - plane.gettimeEntered() >= maxFuel) {
                planesCrashed++;
                landingQueue.deleteQueue();
            }
            else if (currentTime - plane.gettimeEntered() >= landingTime) {
                planesLanded++;
                totalLandingWait += currentTime - plane.gettimeEntered();
                landingQueue.deleteQueue();
            }
        }
        // processing takeoff queue if landing queue is empty. Priority Landing > takeoff.
        else if (!takeoffQueue.isEmptyQueue()) {
            Plane plane = takeoffQueue.front();
            if (currentTime - plane.gettimeEntered() >= takeoffTime) {
                planesTookOff++;
                totalTakeoffWait += currentTime - plane.gettimeEntered();
                takeoffQueue.deleteQueue();
            }
        }

        currentTime++; // Increment the simulation clock
    }

    // Output simulation results
    cout << "Total planes landed: " << planesLanded << endl;
    cout << "Total planes took off: " << planesTookOff << endl;
    cout << "Total planes crashed: " << planesCrashed << endl;

    // Average Landing Wait.
    double averageLandingWait;
    if (planesLanded) {
        averageLandingWait = static_cast<double>(totalLandingWait) / planesLanded;
    }
    else {
        averageLandingWait = 0;
    }
    cout << "Average waiting time for landing: " << averageLandingWait << " minutes" << endl;

    // Average Takeoff Wait.
    double averageTakeoffWait;
    if (planesTookOff) {
        averageTakeoffWait = static_cast<double>(totalTakeoffWait) / planesTookOff;
    }
    else {
        averageTakeoffWait = 0;
    }
    cout << "Average waiting time for takeoff: " << averageTakeoffWait << " minutes" << endl;

}
