/****************
 * COVID-19ABMGuelphS20
 * 05/08/20
 * ver 1.00
 * 
 * This is the class file for the hospital class
 ***************/


#include <iostream>
#include "hospital.hh"

// Constructor
Hospital::Hospital() {
    numberPpl = 0;
    icuCount = 0;
    totalICU = 0;
    totalHospital = 0;
    hospitalOverflow = false;
    icuOverflow = false;
}

void Hospital::freeIcuBeds(int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= (int)hospitalICU.size()) {
        cout << "Trying to free icu out of bounds index" << endl;
        return;
    }

    hospitalICU.erase(hospitalICU.begin() + indexToRemove);
    icuCount--;
    indicateOverflow();
}

void Hospital::freeHospitalBeds(int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= (int)hospitalGeneralWard.size()) {
        cout << "Trying to free hospital out of bounds index" << endl;
        return;
    }
    
    hospitalGeneralWard.erase(hospitalGeneralWard.begin() + indexToRemove);
    numberPpl--;
    indicateOverflow();
}

void Hospital::indicateOverflow() {
    if(numberPpl > totalBedCount) {
        hospitalOverflow = true;
    } else {
        hospitalOverflow = false;
    }

    if(icuCount > icuBedCount) {
        icuOverflow = true;
    } else {
        icuOverflow = false;
    }
}

void Hospital::increaseHospitalCount(Agent* agentToAdd) {
    if (agentToAdd == NULL) {
        cout << "Passing NULL to increase hospital" << endl;
        return;
    }

    hospitalGeneralWard.push_back(agentToAdd);
    numberPpl++;
    totalHospital++;
    indicateOverflow();
}

void Hospital::increaseIcuCount(Agent* agentToAdd) {
    if (agentToAdd == NULL) {
        cout << "Passing NULL to increase ICU" << endl;
        return;
    }
    
    hospitalICU.push_back(agentToAdd);
    icuCount++;
    totalICU++;
    indicateOverflow(); 
}

void Hospital::HospitalTimeStep(double timestep) {
    for (int i = 0; i < (int)hospitalICU.size(); i++) {
        string sirResponse  = hospitalICU[i]->SIRTimeStep(timestep);
        if (sirResponse == "RECOVERAGENT") {
            Agent *recoveredAgent = hospitalICU.at(i);
            hospitalICU.erase(hospitalICU.begin() + i);
            newlyRecovered.push_back(recoveredAgent);
            icuCount--;
        } else if (sirResponse == "DECEASEAGENT") {
            Agent *deceasedAgent = hospitalICU.at(i);
            hospitalICU.erase(hospitalICU.begin() + i);
            newlyDeceased.push_back(deceasedAgent);
            icuCount--;
        }
    }

    for (int i = 0; i < (int)hospitalGeneralWard.size(); i++) {
        string sirResponse  = hospitalGeneralWard[i]->SIRTimeStep(timestep);
        if (sirResponse == "ICUAGENT") {
            Agent *toICU = hospitalGeneralWard.at(i);
            hospitalGeneralWard.erase(hospitalGeneralWard.begin() + i);
            hospitalICU.push_back(toICU);
            icuCount++;
            totalICU++;
            numberPpl--;
        } else if (sirResponse == "RECOVERAGENT") {
            Agent *recoveredAgent = hospitalGeneralWard.at(i);
            hospitalGeneralWard.erase(hospitalGeneralWard.begin() + i);
            newlyRecovered.push_back(recoveredAgent);
            numberPpl--;
        }
    }
}

int Hospital::getTotalBeds() {
    return numberPpl;
}

int Hospital::getIcuBeds() {
    return icuCount;
}

int Hospital::getTotalICUCount() {
    return totalICU;
}

int Hospital::getTotalHospitalCount() {
    return totalHospital;
}
