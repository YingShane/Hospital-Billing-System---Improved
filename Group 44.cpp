/*
*This program has been developed to help a small private hospital and oversee the hospital billing system for their patients. It 
allows the system to process the bills for many patients during their check out from the hospital. 
*Developers: Yeo Ying Sheng, Edu Sinusi, Challven Japirin
*Contact: yeousm@student.usm.my, sinusiedu@student.usm.my, challven001@student.usm.my
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;


const int 	FIRST_CHOICE = 1,
			SECOND_CHOICE = 2,
			THIRD_CHOICE = 3,
			FOURTH_CHOICE = 4;

void mainMenu();
void input(string, string, string[], double[], double *);
double calCategory(double[], int);
double calEachPatientCharges(double[], double[], double[], double[], int);
int findHighest(double[], int);
int findLowest(double[], int);

int main()
{
	int decision, patientNum, lowest, highest, counter=0;
	double 	hospitalStayFee, surgeryFee, medicationFee, serviceFee, overallFee, 
			duration, durationSum = 0, avgHospitalStayCost, avgHospitalStayDays;
	const int CHECKOUT = 5;
	
	//	Initialize the 1-D parallel arrays for hospital stays, surgery, medication and service
	//	The different name/types and charges of each category are stored as elements in the arrays
	string hospitalStay[50];
	double hospitalStayCost[50];
	string meals[50];
	double mealsCost[50];
	string surgery[50];
	double surgeryCost[50];
	string medication[50];
	double medicationCost[50];
	string service[50];
	double serviceCost[50];
	
	//	To determine the number of patients to be checked out
	cout << "Please enter the number of patients: ";
	cin >> patientNum;
	
	
	string patientName[patientNum];
	
	/*	The following arrays are dynamically allocated so that 
		the addresses of these arrays can be passed as arguments 
		to another function to manipulate each element in the arrays */
		
	/*	The arrays are used to store the charges of each category, 
		as an example, in surgeryCharge[patientNum], 
		the first element with subscript 0 is the surgery 
		charges of the first patient								*/
	double *hospitalStayCharge = new double[patientNum];
	double *mealsCharge = new double[patientNum];
	double *surgeryCharge = new double[patientNum];
	double *medicationCharge = new double[patientNum];
	double *serviceCharge = new double[patientNum];
	double patientCharge[patientNum];
	
	system("cls");
	
	//	This for loop will loop as many times as the number of patients to be checked out from the hospital
	for(int i=0; i < patientNum; i++)
	{		
		cout << "Please enter the #" << i+1 << " patient's name: ";
		cin.ignore();
		getline(cin, patientName[i]);
		cout << patientName[i];
		
		system("cls");
	
		do
		{
			mainMenu();
			cout << "Please pick an option: ";
			cin >> decision;
			system("cls");
			
			//	This while loop will loop as long as the decision is out of range allowed
			while(decision > CHECKOUT || decision < FIRST_CHOICE)
			{
				mainMenu();
				cout << "Please pick a valid option: ";
				cin >> decision;
				system("cls");
			}
			
			if(decision != CHECKOUT)
			{
				switch(decision)
				{
					case FIRST_CHOICE:
						/* 	Counter is set to 0 so that this section can only be accessed 
							once per patient as the counter will be incremented later 		*/
						if(counter == 0)
						{
							cout << "Please enter the number of days that the patient has stayed in hospital: ";
							cin >> duration;
							system("cls");
							
							//	Calculate the charge of room of the patient
							input("hospitalStayDetails.txt", "Types of Rooms", hospitalStay, hospitalStayCost, hospitalStayCharge + i);
							system("cls");
							
							/* 	This for loop will be iterated as many times as the 
								number of days that the patient has stayed in hospital
								The meal of that patient for each day is asked and determined */
							for(int j=0; j < duration; j++)
							{
								input("meals.txt", "Types of Meals", meals, mealsCost, mealsCharge + i);
								cout << endl;
							}
							system("cls");
							
							//	Calculate the total number of days for all patients
							durationSum += duration;
							
							/* 	The charge of room of the patient is multiplied by the 
								number of days that the patient has stayed in hospital 
								because that is the cost of room per day 				*/
							hospitalStayCharge[i] *= duration;
							
							/* 	The result of multiplication between charge of room per day and 
								the number of days is added to the charge of meals.
								The meals chosen by patient for each day in hospital is added 	*/
							hospitalStayCharge[i] += mealsCharge[i];

							counter++;
						}
						else
						{
							cout << "This option can only be chosen once for each patient." << endl;
						}
						break;
					case SECOND_CHOICE:
						//	Calculate the surgery charges of each patient
						input("surgery.txt", "Types of Surgery", surgery, surgeryCost, surgeryCharge + i);
						system("cls");
						break;
					case THIRD_CHOICE:
						//	Calculate the medication charges of each patient			
						input("medication.txt", "Types of Medication", medication, medicationCost, medicationCharge + i);
						system("cls");
						break;
					case FOURTH_CHOICE:
						//	Calculate the service charges of each patient
						input("service.txt", "Types of Service", service, serviceCost, serviceCharge + i);
						system("cls");
						break;
					default:
						break;
				}
			}
		}while(decision != CHECKOUT);
		//	Counter is set to zero after the information process of each patient is completed
		counter = 0;
	}
	
	/* 	These functions are called to calculate the sum of each category of 
		hospital stay, surgery, medication and service separately 				*/
	hospitalStayFee = calCategory(hospitalStayCharge, patientNum);
	surgeryFee = calCategory(surgeryCharge, patientNum);
	medicationFee = calCategory(medicationCharge, patientNum);
	serviceFee = calCategory(serviceCharge, patientNum);
	
	//	Calculate the average cost and duration of hospital stays for one patient
	avgHospitalStayCost = hospitalStayFee / patientNum;
	avgHospitalStayDays = durationSum / patientNum;
	
	//	Calculates the total sum of charges of hospital stay, surgery, medication and service
	overallFee = hospitalStayFee + surgeryFee + medicationFee + serviceFee;
	
	//	Calls the function calEachPatientCharges to calculate the total charges of each patient separately
	//	When i is equal to 0, the total charge for the first patient with subscript 0 in the array is calculated
	//	When i is equal to 1, the total charge for the second patient with subscript 1 in the array is calculated and etc
	for(int i=0; i < patientNum; i++)
	{
		patientCharge[i] = calEachPatientCharges(hospitalStayCharge, surgeryCharge, medicationCharge, serviceCharge, i);
	}
	
	/* 	Call the findHighest and findLowest functions to return the array 
		subscript of the patient with highest bill and lowest bill 			*/
	highest = findHighest(patientCharge, patientNum);
	lowest = findLowest(patientCharge, patientNum);
	
	//	This section displays the summary of payment of each patient
	//	This part displays all the necessary information of each patient
	cout << "Summary of Payment for Each Patient" << endl;
	cout << "--------------------------------------------------------" << endl;
	for(int i=0; i < patientNum; i++)
	{
		cout << fixed << setprecision(2);
		cout << "Payment of " << patientName[i] << ":" << endl;
		cout << "Hospital stays charges: RM" << hospitalStayCharge[i] << endl;
		cout << "Surgery charges: RM" << surgeryCharge[i] << endl;
		cout << "Medication charges: RM" << medicationCharge[i] << endl;
		cout << "Service charges: RM" << serviceCharge[i] << endl;
		cout << "Total charges for this patient: RM" << patientCharge[i] << endl;
		cout << "--------------------------------------------------------" << endl;
	}
	cout << endl;
	
	//	This section displays the summary of payment for each category
	cout << "Summary of Payment for Each Category" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Hospital stays charges: RM" << hospitalStayFee << endl;
	cout << "Surgery charges: RM" << surgeryFee << endl;
	cout << "Medication charges: RM" << medicationFee << endl;
	cout << "Service charges: RM" << serviceFee << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << endl;
	
	//	This section displays the summary of overall payment
	cout << "Summary of Overall Payment" << endl;
	cout << "--------------------------------------------------------" << endl;
	
	//	Displays the name of patients with the highest bill and the lowest bill
	cout << "Patient with the highest bill: " << patientName[highest] << endl;
	cout << "Patient with the lowest bill: " << patientName[lowest] << endl;
	cout << "Average hospital stay charges for a patient: RM" << avgHospitalStayCost << endl;
	cout << "Average hospital stay days for a patient: " << avgHospitalStayDays << endl;
	cout << "Total charges for all patients: RM" << overallFee << endl;
	cout << "--------------------------------------------------------" << endl;
	
	// Free dynamically allocated memory
	delete [] hospitalStayCharge;
	hospitalStayCharge = nullptr;
	delete [] mealsCharge;
	mealsCharge = nullptr;
	delete [] surgeryCharge;
	surgeryCharge = nullptr;
	delete [] medicationCharge;
	medicationCharge = nullptr;
	delete [] serviceCharge;
	serviceCharge = nullptr;
	
	return 0;
}



//	This function displays the main menu
void mainMenu()
{
	cout << "Patient Billing Menu\n"
		 << "--------------------------------\n"
		 << "1. Hospital Stays\n"
		 << "2. Types of Surgery\n"
		 << "3. Types of Medication\n"
		 << "4. Types of Service\n"
		 << "5. Check out\n"
		 << "--------------------------------\n";
}



/* 	This function inputs the contents of different categories and cost of each 
	content from the inputFile into the arrays to produce 1-D parallel arrays 	*/
	
/* 	This function also calculates the charge of each category of 
	hospital stay, surgery, medication, and service according 
	to the arguments passed into its parameters 							*/
void input(string fileName, string titleName, string array[], double cost[], double *categoryCharge)
{
	fstream inputFile;
	inputFile.open(fileName, ios::in);
	
	string line;
	int size = 0, option;
	
	getline(inputFile, line, '\n');
	
	/*	Inputs from inputFile until the end of file to determine 
		the number of lines of contents in the file excluding the titles */
	while(!inputFile.eof())
	{
		getline(inputFile, line, '\n');
		size++;
	}

	inputFile.seekg(0, ios::beg);
	getline(inputFile, line, '\n');
	
	//	The contents of inputFile are being input into the arrays of array and cost
	for(int i=0; i < size; i++)
	{	
		getline(inputFile, array[i], '\t');
		inputFile >> cost[i];	
		getline(inputFile, line, '\n');
	}
	
	/*	This section displays the menu of different categories like the 
		types of rooms chosen by patient, types of meals, types of surgeries, types of medications, 
		and types of services according to what arguments are passed into the parameters 				*/
	cout << titleName << endl;
	cout << "-----------------------------" << endl;
	
	int i=0;
	for(; i < size; i++)
	{
		cout << i+1 << ". " << array[i] << endl;
	}
	cout << i+1 << ". " << "Check Out" << endl;
	cout << "-----------------------------" << endl;
	cout << "Please pick an option (" << FIRST_CHOICE << " - " << size+1 << "): ";
	cin >> option;
	
	//	This section will execute as long as the option is out of range
	while(option < FIRST_CHOICE || option > size+1)
	{
		system("cls");
		cout << titleName << endl;
		cout << "-----------------------------" << endl;
		int i=0;
		for(; i < size; i++)
		{
			cout << i+1 << ". " << array[i] << endl;
		}
		cout << i+1 << ". " << "Check Out" << endl;
		cout << "-----------------------------" << endl;
		cout << "Please pick an option (" << FIRST_CHOICE << " - " << size+1 << "): ";
		cin >> option;
	}
	
	/* 	Put the charges of each category of hospital stay, 
		surgery, medication, and service into different arrays */
		
	//	This is the part where the arrays are being filled with elements to be processed later
	*categoryCharge += cost[option-1];
	
	
	inputFile.close();
}



/* 	This function calculates and returns the total charges of each 
	category of hospital stay, surgery, medication and service separately */
double calCategory(double categoryCharge[], int patientNum)
{
	double sum = 0;
	for(int i=0; i < patientNum; i++)
	{
		sum += categoryCharge[i];
	}
	return sum;
}



/* 	Calculates and returns the sum of hospital stay charges, surgery charges, 
	medication charges and service charges for each patient separately 			*/
//	If num = 0, the total bills of the first patient with subscript 0 in the array is returned
//	If num = 1, the total bills of the second patient with subscript 1 in the array is returned and etc
double calEachPatientCharges(double hospitalStayCharge[], double surgeryCharge[], double medicationCharge[], double serviceCharge[], int num)
{
	return hospitalStayCharge[num] + surgeryCharge[num] + medicationCharge[num] + serviceCharge[num];
}



//	Returns the array subscript of the highest bill to be paid to the hospital
int findHighest(double patientCharge[], int patientNum)
{
	int max = patientCharge[0], maxSubscript = 0;
	for(int i=0; i < patientNum; i++)
	{
		if(max < patientCharge[i])
		{
			max = patientCharge[i];
			maxSubscript = i;
		}
	}
	return maxSubscript;
}



//	Returns the array subscript of the lowest bill to be paid to the hospital
int findLowest(double patientCharge[], int patientNum)
{
	int min = patientCharge[0], minSubscript = 0;
	for(int i=0; i < patientNum; i++)
	{
		if(min > patientCharge[i])
		{
			min = patientCharge[i];
			minSubscript = i;
		}
	}
	return minSubscript;
}






















