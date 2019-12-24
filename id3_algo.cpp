#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <time.h> 
#include <math.h>

using namespace std;

// Structure for the node of the decision tree
struct Node {
	string feature;
	double threshhold;
	Node *below;
	Node *above;
	int current_guess;
};

// Function that read the classification of the available dataset and return the class with the majority count
// VECTOR<INT> -> INT
int find_best_class(vector<int> classification) {
	// count for setosa
	int setosa = 0;
	// count for versicolor
	int versicolor = 0;
	// count for virginica
	int virginica = 0;
	// go through the data
	for (int i = 0; i < classification.size(); i++) {
		if (classification[i] == 0) {
			setosa++;
		} else if (classification[i] == 1) {
			versicolor++;
		} else if (classification[i] == 2) {
			virginica++;
		} else {
			cout << "Invalid class of " << classification[i] << ". Returning -1" << endl;
			return -1;
		}
	}
	// Return the class with largest count
	if ((setosa > versicolor) && (setosa > virginica)) {
		return 0;
	} else if ((virginica > versicolor) && (setosa < virginica)) {
		return 2;
	} else if ((setosa < versicolor) && (versicolor > virginica)) { 
		return 1;
	}

	//cout << "You shouldn't come here" << endl;

	// If there is tie
	srand(time(NULL));
	int min = 0;
	int max = 99;
	int random = (min + (rand() % (int)(max - min + 1)));
	if ((setosa == versicolor) && (setosa == virginica)) { 
		// break tie randomly between the three
		if (random < 33) {
			return 0;
		} else if (random < 66) {
			return 1;
		} else {
			return 2;
		}
	} else if (setosa == versicolor) {
		// break tie randomly between the two
		if (random < 50) {
			return 0;
		} else {
			return 1;
		}
	} else if (setosa == virginica) {
		// break tie randomly between the two
		if (random < 50) {
			return 0;
		} else {
			return 2;
		}
	} else {
		// Break tie randomly between the two
		if (random < 50) {
			return 1;
		} else {
			return 2;
		}
	}
}
	
// Function that see if the calssification vector holds the same value throughout
// VECTOR<INT> -> BOOL
bool Same_Class(vector<int> classification) {
	int c = classification[0];
	for (int i = 1; i < classification.size(); i++) {
		if (classification[i] != c) {
			return false;
		}
	}
	return true;
}

// Function that count and return the number of data correspind to the provided integer
// VECTOR<INT> INT -> INT
int CountClass(vector<int> classification, int class_to_count) {
	// the count to return
	int count = 0;
	// Go trhough the vector and count the corespinding class
	for (int i = 0; i < classification.size(); i++) {
		if (classification[i] == class_to_count) {
			count++;
		}
	}
	return count;
}

// Function that constructed the sorted pair of list from a feature and the corrresponding class
// Requre: the two argument need to be the same size
// VECTOR<DOUBLE> VECTOR<INT> -> VECTOR<PAIR<DOUBLE,INT>>
vector<pair<double,int> > GetSortedPairs(vector<double> feature, vector<int> classification) {
	// vector to return
	vector<pair<double,int> > sp;
	// iterate through the data and reconstruct it into the new vector
	for (int i = 0; i < feature.size(); i++) {
		pair<double,int> p;
		p.first = feature[i];
		p.second = classification[i];
		if (sp.empty()) {
			sp.push_back(p);
			//-------------------------------
			//cout << sp[0].first << endl;
			//-------------------------------
		} else {
			for (int j = 0; j < sp.size(); j++) {
				if (j == sp.size()-1) {
					if (p.first >= sp[j].first) {
						sp.push_back(p);
						//----------------------------------------------------------
						//for (int z = 0; z < sp.size(); z++) {
						//	cout << sp[z].first << " ";
						//}
						//cout << endl;
						//---------------------------------------------------------
						break;
					} else {
						sp.insert(sp.begin()+j,p);
						//----------------------------------------------------------
                                                //for (int z = 0; z < sp.size(); z++) {
                                                //        cout << sp[z].first << " ";
                                                //}
                                                //cout << endl;
                                                //---------------------------------------------------------
						break;
					}
				} else if (p.first < sp[j].first) {
					sp.insert(sp.begin()+j,p);
					//----------------------------------------------------------
                                        //for (int z = 0; z < sp.size(); z++) {
					//	cout << sp[z].first << " ";
					//}
					//cout << endl;
                                        //---------------------------------------------------------
					break;
				}
			}
		}
	}
	return sp;
}

// Function to get the possible splitpoint from the sorted list
// VECTOR<PAIR<DOUBLE,INT>> -> VECTOR<DOUBLE>
vector<double> GetThreshholds(vector<pair<double,int> > SortedPairs) {
	//----------------------------------------------------------------------------------------------------
	//cout << "In the GetThredhholds function" << endl;
	//cout << "The SortedPairs has the size of: " << SortedPairs.size() << endl;
	//---------------------------------------------------------------------------------------------------
	// The vector to return
	vector<double> threshholds;

	// Iterate through the sorted pairs and add the split points on th way
	for (int i = 0; i < SortedPairs.size(); i++) {
		if (i == SortedPairs.size()-1) {
			//--------------------------------------------------------------
			//cout << "This is the last element. Break!!" << endl;
			//--------------------------------------------------------------
			break;
		}
		//---------------------------------------------------------------------------------------
		//cout << i << "th element for the start point" << endl;
		//cout << "feature: " << SortedPairs[i].first << endl;
		//cout << "class: " << SortedPairs[i].second << endl;
		//---------------------------------------------------------------------------------------
		// Find next data that differ in feature
		int j = i+1;
		//---------------------------------------------------------------------------------------
                //cout << j << "th element for the end point" << endl;
                //cout << "feature: " << SortedPairs[j].first << endl;
                //cout << "class: " << SortedPairs[j].second << endl;
                //---------------------------------------------------------------------------------------
		while ((SortedPairs[i].first == SortedPairs[j].first) && (j < SortedPairs.size())) {
			j++;
			//----------------------------------------------------------------
			//cout << "Same feature" << endl;
                        //cout << j << "th element for the end point" << endl;
                        //cout << "feature: " << SortedPairs[j].first << endl;
                        //cout << "class: " << SortedPairs[j].second << endl;
                        //----------------------------------------------------------------
		}
		// Simply add the split point - this can be improve later on by looking at the classes
		double split = (SortedPairs[i].first + ((SortedPairs[j].first - SortedPairs[i].first)/2.0));
		threshholds.push_back(split);
		//--------------------------------------------------------------------------------------------
		//cout << "Pushed the split point: " << split << endl;
		//cout << "Update the start point index" << endl;
		//--------------------------------------------------------------------------------------------
		i = j-1;
	}
	// Return the possible split points
	return threshholds;
}

// Function that split the current data on the threshhold and return the expected entropy
// The size of feature and classification need to be same
// DOUBLE VECTOR<DOUBEL> VECTOR<INT> -> DOUBLE
double GetExpectedEentropy(double threshhold, vector<double> feature, vector<int> classification) {
	//-----------------------------------------------------------------------------------------------------------
	//cout << "In GetExpectedEntropy function" << endl;
	//cout << "Threshhold: " << threshhold << endl;
	//cout << "Size of feature: " << feature.size() << endl;
	//-----------------------------------------------------------------------------------------------------------
	// the total number of the original data
	int original_size = classification.size();
	// split the data into two new set
	vector<int> class_one;
	vector<int> class_two;
	for (int i = 0; i < feature.size(); i++) {
		if (feature[i] < threshhold) {
			class_one.push_back(classification[i]);
		} else {
			class_two.push_back(classification[i]);
		}
	}
	// size of the new two data sets
	int new_size_one = class_one.size();
	int new_size_two = class_two.size();
	// Calculate the entropy for the first set (below threshhold)
	int number_of_setosa = CountClass(class_one,0);
        int number_of_versicolor = CountClass(class_one,1);
        int number_of_virginica = CountClass(class_one,2);
	//------------------------------------------------------------------------------
	//cout << "new_size_one: " << new_size_one << endl;
	//cout << "number_of_setosa: " << number_of_setosa << endl;
	//cout << "number_of_versicolor: " << number_of_versicolor << endl;
	//cout << "number_of_virginica: " << number_of_virginica << endl;
	//------------------------------------------------------------------------------
	double log_setosa = 0.0;
	if (number_of_setosa != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
		//cout << "(" << number_of_setosa << "/" << new_size_one << ")" << "log(" << number_of_setosa << "/" << new_size_one << ")" << endl;
		//---------------------------------------------------------------------------------------------------------------------------------------
		log_setosa = (((double)number_of_setosa/(double)new_size_one)*(log2(((double)number_of_setosa/(double)new_size_one))));
	}
	double log_versicolor = 0.0;
	if (number_of_versicolor != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
                //cout << "(" << number_of_versicolor << "/" << new_size_one << ")" << "log(" << number_of_versicolor << "/" << new_size_one << ")" << endl;
                //---------------------------------------------------------------------------------------------------------------------------------------
		log_versicolor = (((double)number_of_versicolor/(double)new_size_one)*(log2(((double)number_of_versicolor/(double)new_size_one))));
	}
	double log_virginica = 0.0;
	if (number_of_virginica != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
                //cout << "(" << number_of_virginica << "/" << new_size_one << ")" << "log(" << number_of_virginica << "/" << new_size_one << ")" << endl;
                //---------------------------------------------------------------------------------------------------------------------------------------
		log_virginica = (((double)number_of_virginica/new_size_one)*(log2(((double)number_of_virginica/(double)new_size_one))));
	}
	double class_one_entropy = (-1.0 * (log_setosa + log_versicolor + log_virginica));
	//------------------------------------------------------------------------------------------------------------------------------
        //cout << "-1.0 * (" << log_setosa << "+" << log_versicolor << "+" << log_virginica << ")" << " = " << class_one_entropy << endl;
	//------------------------------------------------------------------------------------------------------------------------------
	// Calculate the entropy for the second set (above threshhold)
	number_of_setosa = CountClass(class_two,0);
        number_of_versicolor = CountClass(class_two,1);
        number_of_virginica = CountClass(class_two,2);
	//------------------------------------------------------------------------------
        //cout << "new_size_two: " << new_size_one << endl;
        //cout << "number_of_setosa: " << number_of_setosa << endl;
        //cout << "number_of_versicolor: " << number_of_versicolor << endl;
        //cout << "number_of_virginica: " << number_of_virginica << endl;
        //------------------------------------------------------------------------------
	log_setosa = 0.0;
        if (number_of_setosa != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
                //cout << "(" << number_of_setosa << "/" << new_size_two << ")" << "log(" << number_of_setosa << "/" << new_size_two << ")" << endl;
                //---------------------------------------------------------------------------------------------------------------------------------------
                log_setosa = (((double)number_of_setosa/(double)new_size_two)*(log2(((double)number_of_setosa/(double)new_size_two))));
        }
        log_versicolor = 0.0;
        if (number_of_versicolor != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
                //cout << "(" << number_of_versicolor << "/" << new_size_two << ")" << "log(" << number_of_versicolor << "/" << new_size_two << ")" << endl;
                //---------------------------------------------------------------------------------------------------------------------------------------
                log_versicolor = (((double)number_of_versicolor/(double)new_size_two)*(log2(((double)number_of_versicolor/(double)new_size_two))));
        }
        log_virginica = 0.0;
        if (number_of_virginica != 0) {
		//---------------------------------------------------------------------------------------------------------------------------------------
                //cout << "(" << number_of_virginica << "/" << new_size_two << ")" << "log(" << number_of_virginica << "/" << new_size_two << ")" << endl;
                //---------------------------------------------------------------------------------------------------------------------------------------
                log_virginica = (((double)number_of_virginica/(double)new_size_two)*(log2(((double)number_of_virginica/(double)new_size_two))));
        }
        double class_two_entropy = (-1.0 * (log_setosa + log_versicolor + log_virginica));
	//---------------------------------------------------------------------------------------------------------------------
        //cout << "-1.0 * (" << log_setosa << "+" << log_versicolor << "+" << log_virginica << ")" << " = " << class_two_entropy;
        //---------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//cout << new_size_one << "/" << original_size << "*" << class_one_entropy << "+"  << new_size_two << "/" << original_size << "*" << class_two_entropy << endl;
	//cout << "Returning entropy: " << ((((double)new_size_one/(double)original_size)*(class_one_entropy))+(((double)new_size_two/(double)original_size)*(class_two_entropy))) << endl;
	//------------------------------------------------------------------------------------------------------------------------------------------------
	// return the expected antropy accounting the two set
	return ((((double)new_size_one/(double)original_size)*(class_one_entropy))+(((double)new_size_two/(double)original_size)*(class_two_entropy)));
}

// Recursive function that create the decision tree from the training set until the specified depth. Depth will be ignore if negative
// VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<INT> INT -> NODE*
Node * MakeDecisionTree(vector<double> sepal_length, vector<double> sepal_width, vector<double> petal_length, 
			vector<double> petal_width, vector<int> classification, int depth) {
	//----------------------------------------------------------------------------------------------------------------------------------------
	//cout << "sepal_length_size: " << sepal_length.size() << endl;
	//cout << "sepal_width_size: " << sepal_width.size() << endl;
	//cout << "petal_length_size: " << petal_length.size() << endl;
	//cout << "petal_width_size: " << petal_width.size() << endl;
	//cout << "classification_size: " << classification.size() << endl;
	//----------------------------------------------------------------------------------------------------------------------------------------

	// new node to return
        Node *n = new Node;

	// If we have an answer or we reach the maximum depth
	if ((Same_Class(classification)) || (depth == 0)) {
		//-------------------------------------------------------------------------------------------
		//cout << "They are all same class!!" << endl;
		//------------------------------------------------------------------------------------------
		n->feature = "NA";
		n->threshhold = 0;
		n->below = NULL;
		n->above = NULL;
		n->current_guess = find_best_class(classification);
		if (n->current_guess == -1) {
			cout << "Undefined class, the data is broken" << endl;
		}
		return n;
	}

	// For each feature, choose a threhold and calculate the maximum gain for each. Then compare in turns of feature to decide on feature
	// Current entropy
	int total_data_point = classification.size();
	int number_of_setosa = CountClass(classification,0);
	int number_of_versicolor = CountClass(classification,1);
	int number_of_virginica = CountClass(classification,2);
	double log_setosa = 0.0;
	if (number_of_setosa != 0) {
		log_setosa = (((double)number_of_setosa/(double)total_data_point)*(log2((double)number_of_setosa/(double)total_data_point)));
	}
	double log_versicolor = 0.0;
	if (number_of_versicolor != 0) {
		log_versicolor = (((double)number_of_versicolor/(double)total_data_point)*(log2((double)number_of_versicolor/(double)total_data_point)));
	}
	double log_virginica = 0.0;
	if (number_of_virginica != 0) {
		log_virginica = (((double)number_of_virginica/(double)total_data_point)*(log2((double)number_of_virginica/(double)total_data_point)));
	}
	double current_entropy = (-1.0 * (log_setosa + log_versicolor + log_virginica));
	//-----------------------------------------------------------------------------------------
	//cout << "current entropy: " << current_entropy << endl;
	//----------------------------------------------------------------------------------------
	// Threshold for whatever feature that we decide to use
	double threshhold = -1;
	// maximum gain so far
	double max_gain_so_far = -1;
	// Data to keep track of the feature threshhold combination that procuce the max gain
	vector<pair<string,double> > options;
	options.clear();

	// Test the first feature
	double sepal_length_max_gain;
	// Sort sepal_length vector and make a dictionary pair with correspinding class
	vector<pair<double,int> > sepal_length_sort = GetSortedPairs(sepal_length,classification);
	//-----------------------------------------------------------------------------------------------------
	//cout << "The sorted pairs are" << endl;
	//for (int z = 0; z < sepal_length_sort.size(); z++) {
	//	cout << "(" << sepal_length_sort[z].first << "," << sepal_length_sort[z].second << ")" << endl;
	//}
	//-----------------------------------------------------------------------------------------------------
	// List of splitpoints. It will get recycle when testing a new feature
        vector<double> threshholds = GetThreshholds(sepal_length_sort);
	//-----------------------------------------------------------------------------------------------------
	//cout << "The split points are" << endl;
	//for (int z = 0; z < threshholds.size(); z++) {
        //        cout << threshholds[z] << " ";
        //}
	//cout << endl;
	//-----------------------------------------------------------------------------------------------------
	// Calculate the maximum gain out of the all possible split points
	double expected_entropy;
	double gain;
	if (!threshholds.empty()) {
		expected_entropy = GetExpectedEentropy(threshholds[0],sepal_length,classification);
		//-------------------------------------------------------------------------------------
		//cout << "GetExpectedEntropy Done at first feature" << endl;
		//-------------------------------------------------------------------------------------
		gain = current_entropy - expected_entropy;
		sepal_length_max_gain = gain;
		threshhold = threshholds[0];
		// For each splitpoints, calculate the maximum gain
		for (int i = 1; i < threshholds.size(); i++) {
			expected_entropy = GetExpectedEentropy(threshholds[i],sepal_length,classification);
			//-------------------------------------------------------------------------------------
	                //cout << "GetExpectedEntropy Done at first feature in loop" << endl;
        	        //-------------------------------------------------------------------------------------
			gain = current_entropy - expected_entropy;
			if (gain > sepal_length_max_gain) {
				sepal_length_max_gain = gain;
				threshhold = threshholds[i];
			} else if (gain == sepal_length_max_gain) {
                                // There is 50% chance of getting this threshhold (chose randomly)
                                srand(time(NULL));
                                int min = 0;
                                int max = 99;
                                int random = (min + (rand() % (int)(max - min)));
                                if (random < 50) {
                                        threshhold = threshholds[i];
                                }
                        }
		}
		max_gain_so_far = sepal_length_max_gain;
		pair<string,double> p;
		p.first = "Sepal Length";
		p.second = threshhold;
		options.push_back(p);
		
	}
	//------------------------------------------------------------------------------------------------------
	//cout << "The maximum gain for sepal_length is: " << sepal_length_max_gain << endl;
	//cout << "With threshhold: " << threshhold << endl;
	//-----------------------------------------------------------------------------------------------------

	// Test the second feature
	double sepal_width_max_gain;
	threshholds.clear();
	// Sort sepal_width vector and make a dictionary pair with correspinding class
        vector<pair<double,int> > sepal_width_sort = GetSortedPairs(sepal_width,classification);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The sorted pairs are" << endl;
        //for (int z = 0; z < sepal_width_sort.size(); z++) {
        //        cout << "(" << sepal_width_sort[z].first << "," << sepal_width_sort[z].second << ")" << endl;
        //}
        //-----------------------------------------------------------------------------------------------------
	// List of splitpoints. It will get recycle when testing a new feature
        threshholds = GetThreshholds(sepal_width_sort);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The split points are" << endl;
        //for (int z = 0; z < threshholds.size(); z++) {
        //        cout << threshholds[z] << " ";
        //}
        //cout << endl;
        //-----------------------------------------------------------------------------------------------------
	// Calculate the maximum gain out of the all possible split points
        if (!threshholds.empty()) {
                expected_entropy = GetExpectedEentropy(threshholds[0],sepal_width,classification);
		//-------------------------------------------------------------------------------------
                //cout << "GetExpectedEntropy Done at second feature" << endl;
                //-------------------------------------------------------------------------------------
                gain = current_entropy - expected_entropy;
                sepal_width_max_gain = gain;
		// Update the threshhold
                threshhold = threshholds[0];
                // For each splitpoints, calculate the maximum gain
                for (int i = 1; i < threshholds.size(); i++) {
                        expected_entropy = GetExpectedEentropy(threshholds[i],sepal_width,classification);
			//-------------------------------------------------------------------------------------
	                //cout << "GetExpectedEntropy Done at second feature in loop" << endl;
        	        //-------------------------------------------------------------------------------------
                        gain = current_entropy - expected_entropy;
                        if (gain > sepal_width_max_gain) {
                                sepal_width_max_gain = gain;
				threshhold = threshholds[i];
                        } else if (gain == sepal_width_max_gain) {
                                // There is 50% chance of getting this threshhold (chose randomly)
                                srand(time(NULL));
                                int min = 0;
                                int max = 99;
                                int random = (min + (rand() % (int)(max - min)));
                                if (random < 50) {
                                        threshhold = threshholds[i];
                                }
                        }
                }
		if (!options.empty()) {
			if (max_gain_so_far < sepal_width_max_gain) {
                                options.clear();
			}
		}
		if (max_gain_so_far <= sepal_width_max_gain) {
			max_gain_so_far = sepal_width_max_gain;
                        pair<string,double> p;
                        p.first = "Sepal Width";
                        p.second = threshhold;
                        options.push_back(p);
		}
        }
	//------------------------------------------------------------------------------------------------------
        //cout << "The maximum gain for sepal_width is: " << sepal_width_max_gain << endl;
        //cout << "With threshhold: " << threshhold << endl;
        //-----------------------------------------------------------------------------------------------------
        
	// Test the third feature
	double petal_length_max_gain;
	threshholds.clear();
	// Sort petal_length vector and make a dictionary pair with correspinding class
        vector<pair<double,int> > petal_length_sort = GetSortedPairs(petal_length,classification);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The sorted pairs are" << endl;
        //for (int z = 0; z < petal_length_sort.size(); z++) {
        //        cout << "(" << petal_length_sort[z].first << "," << petal_length_sort[z].second << ")" << endl;
        //}
        //-----------------------------------------------------------------------------------------------------
        // List of splitpoints. It will get recycle when testing a new feature
        threshholds = GetThreshholds(petal_length_sort);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The split points are" << endl;
        //for (int z = 0; z < threshholds.size(); z++) {
        //        cout << threshholds[z] << " ";
        //}
        //cout << endl;
        //-----------------------------------------------------------------------------------------------------
	// Calculate the maximum gain out of the all possible split points
        if (!threshholds.empty()) {
                expected_entropy = GetExpectedEentropy(threshholds[0],petal_length,classification);
		//-------------------------------------------------------------------------------------
                //cout << "GetExpectedEntropy Done at third feature" << endl;
                //-------------------------------------------------------------------------------------
                gain = current_entropy - expected_entropy;
                petal_length_max_gain = gain;
                // Update the threshhold
		threshhold = threshholds[0];
                // For each splitpoints, calculate the maximum gain
                for (int i = 1; i < threshholds.size(); i++) {
                        expected_entropy = GetExpectedEentropy(threshholds[i],petal_length,classification);
			//-------------------------------------------------------------------------------------
        	        //cout << "GetExpectedEntropy Done at third feature in loop" << endl;
	                //-------------------------------------------------------------------------------------
                        gain = current_entropy - expected_entropy;
                        if (gain > petal_length_max_gain) {
                                petal_length_max_gain = gain;
				threshhold = threshholds[i];
                        } else if (gain ==  petal_length_max_gain) {
                                // There is 50% chance of getting this threshhold (chose randomly)
                                srand(time(NULL));
                                int min = 0;
                                int max = 99;
                                int random = (min + (rand() % (int)(max - min)));
                                if (random < 50) {
                                        threshhold = threshholds[i];
                                }
                        }
                }
		if (!options.empty()) {
                        if (max_gain_so_far < petal_length_max_gain) {
                                options.clear();
                        }
                }
                if (max_gain_so_far <= petal_length_max_gain) {
                        max_gain_so_far = petal_length_max_gain;
                        pair<string,double> p;
                        p.first = "Petal Length";
                        p.second = threshhold;
                        options.push_back(p);
                }
        }
	//------------------------------------------------------------------------------------------------------
        //cout << "The maximum gain for petal_length is: " << petal_length_max_gain << endl;
        //cout << "With threshhold: " << threshhold << endl;
        //-----------------------------------------------------------------------------------------------------

	// Test the fourth feature
	double petal_width_max_gain;
	threshholds.clear();
	// Sort petal_length vector and make a dictionary pair with correspinding class
        vector<pair<double,int> > petal_width_sort = GetSortedPairs(petal_width,classification);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The sorted pairs are" << endl;
        //for (int z = 0; z < petal_width_sort.size(); z++) {
        //        cout << "(" << petal_width_sort[z].first << "," << petal_width_sort[z].second << ")" << endl;
        //}
        //-----------------------------------------------------------------------------------------------------
        // List of splitpoints. It will get recycle when testing a new feature
        threshholds = GetThreshholds(petal_width_sort);
	//-----------------------------------------------------------------------------------------------------
        //cout << "The split points are" << endl;
        //for (int z = 0; z < threshholds.size(); z++) {
        //        cout << threshholds[z] << " ";
        //}
        //cout << endl;
        //-----------------------------------------------------------------------------------------------------
        // Calculate the maximum gain out of the all possible split points
        if (!threshholds.empty()) {
                expected_entropy = GetExpectedEentropy(threshholds[0],petal_width,classification);
		//-------------------------------------------------------------------------------------
                //cout << "GetExpectedEntropy Done at fourth feature" << endl;
                //-------------------------------------------------------------------------------------
                gain = current_entropy - expected_entropy;
                petal_width_max_gain = gain;
                // Update the threshhold
		threshhold = threshholds[0];
                // For each splitpoints, calculate the maximum gain
                for (int i = 1; i < threshholds.size(); i++) {
                        expected_entropy = GetExpectedEentropy(threshholds[i],petal_width,classification);
			//-------------------------------------------------------------------------------------
	                //cout << "GetExpectedEntropy Done at fourth feature in loop" << endl;
        	        //-------------------------------------------------------------------------------------
                        gain = current_entropy - expected_entropy;
                        if (gain > petal_width_max_gain) {
				petal_width_max_gain = gain;
				threshhold = threshholds[i];
                        } else if (gain ==  petal_width_max_gain) {
				// There is 50% chance of getting this threshhold (chose randomly)
				srand(time(NULL));
        			int min = 0;
        			int max = 99;
        			int random = (min + (rand() % (int)(max - min)));			
				if (random < 50) {
					threshhold = threshholds[i];
				}
			}
                }
		if (!options.empty()) {
                        if (max_gain_so_far < petal_width_max_gain) {
                                options.clear();
                        }
                }
                if (max_gain_so_far <= petal_width_max_gain) {
                        max_gain_so_far = petal_width_max_gain;
                        pair<string,double> p;
                        p.first = "Petal Width";
                        p.second = threshhold;
                        options.push_back(p);
                }
        }
	//------------------------------------------------------------------------------------------------------
        //cout << "The maximum gain for petal_width is: " << petal_width_max_gain << endl;
        //cout << "With threshhold: " << threshhold << endl;
        //-----------------------------------------------------------------------------------------------------

	// Split the data according to the derived feature and threshhold
	vector<double> first_sepal_length;
	vector<double> first_sepal_width;
	vector<double> first_petal_length;
	vector<double> first_petal_width;
	vector<int> first_classification;
	vector<double> second_sepal_length;
        vector<double> second_sepal_width;
        vector<double> second_petal_length;
        vector<double> second_petal_width;
        vector<int> second_classification;
	// Chose feature randomly
	string best_feature = "";
	int number_of_options = options.size();
	double point;
	if (number_of_options == 1) {
		best_feature = options[0].first;
		point = options[0].second;
	} else {
		srand(time(NULL));
		int random = (rand() % (int)number_of_options);
		best_feature = options[random].first;
		point = options[random].second;
	}
        //------------------------------------------------------------------
	//cout << "Feature: " << best_feature << endl;
	//cout << "With threshhold: " << point << endl;
	//------------------------------------------------------------------
	if (best_feature == "Petal Width") {	// If petal width is the best feature
		for (int i = 0; i < petal_width.size(); i++) {
			if (petal_width[i] < point) {
				first_sepal_length.push_back(sepal_length[i]);
				first_sepal_width.push_back(sepal_width[i]);
				first_petal_length.push_back(petal_length[i]);
				first_petal_width.push_back(petal_width[i]);
				first_classification.push_back(classification[i]);
			} else {
				second_sepal_length.push_back(sepal_length[i]);
                                second_sepal_width.push_back(sepal_width[i]);
                                second_petal_length.push_back(petal_length[i]);
                                second_petal_width.push_back(petal_width[i]);
                                second_classification.push_back(classification[i]);
			}
		}
		n->feature = "Petal Width";
	} else if (best_feature == "Petal Length") {	// If petal length is the best one
		for (int i = 0; i < petal_length.size(); i++) {
                        if (petal_length[i] < point) {
                                first_sepal_length.push_back(sepal_length[i]);
                                first_sepal_width.push_back(sepal_width[i]);
                                first_petal_length.push_back(petal_length[i]);
                                first_petal_width.push_back(petal_width[i]);
                                first_classification.push_back(classification[i]);
                        } else {
                                second_sepal_length.push_back(sepal_length[i]);
                                second_sepal_width.push_back(sepal_width[i]);
                                second_petal_length.push_back(petal_length[i]);
                                second_petal_width.push_back(petal_width[i]);
                                second_classification.push_back(classification[i]);
                        }
		}
		n->feature = "Petal Length";
	} else if (best_feature == "Sepal Width") {	// If sepal width is the best feature
		for (int i = 0; i < sepal_width.size(); i++) {
                        if (sepal_width[i] < point) {
                                first_sepal_length.push_back(sepal_length[i]);
                                first_sepal_width.push_back(sepal_width[i]);
                                first_petal_length.push_back(petal_length[i]);
                                first_petal_width.push_back(petal_width[i]);
                                first_classification.push_back(classification[i]);
                        } else {
                                second_sepal_length.push_back(sepal_length[i]);
                                second_sepal_width.push_back(sepal_width[i]);
                                second_petal_length.push_back(petal_length[i]);
                                second_petal_width.push_back(petal_width[i]);
                                second_classification.push_back(classification[i]);
                        }
                }
		n->feature = "Sepal Width";
	} else {	// If sepal length is the best one
		for (int i = 0; i < sepal_length.size(); i++) {
                        if (sepal_length[i] < point) {
                                first_sepal_length.push_back(sepal_length[i]);
                                first_sepal_width.push_back(sepal_width[i]);
                                first_petal_length.push_back(petal_length[i]);
                                first_petal_width.push_back(petal_width[i]);
                                first_classification.push_back(classification[i]);
                        } else {
                                second_sepal_length.push_back(sepal_length[i]);
                                second_sepal_width.push_back(sepal_width[i]);
                                second_petal_length.push_back(petal_length[i]);
                                second_petal_width.push_back(petal_width[i]);
                                second_classification.push_back(classification[i]);
                        }
                }
		n->feature = "Sepal Length";
	}

	//----------------------------------------------------------------------------------------------------------
	//cout << "Recursive call" << endl;
	//cout << "Current-feature: " << n->feature << endl;
	//cout << "With threshhold: " << point << endl;
	//cout << "current sepal length" << endl;
	//for (int z = 0; z < sepal_length.size(); z++) {
	//	cout << sepal_length[z] << " ";
	//}
	//cout << endl;
	//cout << "current sepal width" << endl;
        //for (int z = 0; z < sepal_width.size(); z++) {
        //        cout << sepal_width[z] << " ";
        //}
	//cout << endl;
	//cout << "current petal length" << endl;
        //for (int z = 0; z < petal_length.size(); z++) {
        //        cout << petal_length[z] << " ";
        //}
	//cout << endl;
	//cout << "current petal width" << endl;
        //for (int z = 0; z < petal_width.size(); z++) {
        //        cout << petal_width[z] << " ";
        //}
	//cout << endl;
	//cout << "left(below): " << endl;
	//cout << "Classes size: " << first_classification.size() << endl;
	//cout << "right(above): " << endl;
	//cout << "Classes size: " << second_classification.size() << endl;
	//---------------------------------------------------------------------------------------------------------

	// Get the children (leaf)
        Node *below = MakeDecisionTree(first_sepal_length,first_sepal_width,first_petal_length,first_petal_width,first_classification,depth-1);
        Node *above = MakeDecisionTree(second_sepal_length,second_sepal_width,second_petal_length,second_petal_width,second_classification,depth-1);
	
	// update the node and then return i
	n->threshhold = point;
	n->below = below;
	n->above = above;
	n->current_guess = find_best_class(classification);
	//n->current_guess = 0;
	if (n->current_guess == -1) {
		cout << "Undefined class, the data is broken" << endl;
	}
	return n;
}

// Recursive function that print out the tree given the node
// Node* STRING -> VOID
void PrintDecisionTree(Node *root, string level) {
	// Print the feature 
	cout << level << root->feature << endl;
	// Print the threshhold
	cout << level << root->threshhold << endl;
	// Print the current decision
	int c = root->current_guess;
	if (c == 0) {
		cout << level << "setosa" << endl;
	} else if (c == 1) {
		cout << level << "versicolor" << endl;
	} else {
		cout << level << "virginica" << endl;
	}
	cout << level << endl;
	// Print the left sub tree
	if (root->below != NULL) {
		PrintDecisionTree(root->below,level+"|    ");
	}
	// Print the right sub tree
	if (root->above != NULL) {
		PrintDecisionTree(root->above,level+"|    ");
	}
	return;
}

// Recursive function that delete the tree and frees the memory
// Node* -> VOID
void DeleteDecisionTree(Node *root) {
	if (root->below != NULL) {
		DeleteDecisionTree(root->below);
	}
	if (root->above != NULL) {
		DeleteDecisionTree(root->above);
	}
	delete root;
	return;
}

// Function that get the accuracy of the decision tree pased on the provided data
// VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<DOUBLE> VECTOR<INT> NODE* -> Double
double GetAccuracy(vector<double> sepal_length, vector<double> sepal_width, vector<double> petal_length, 
						vector<double> petal_width, vector<int> classification, Node *root) {
	// The size if the data
	int data_size = classification.size();
	if (data_size == 0) {
		cout << "Error: there is no data provided. Returning -1" << endl;
		return -1.0;
	}
	// Number of error made
	int correct = 0;
	// Walk through the tree for each data entry and look if the result match
	for (int i = 0; i < data_size; i++) {
		// Travel the tree
		Node *current = root;
		while ((current->below != NULL) || (current->above != NULL)) {
			double threshhold = current->threshhold;
			string feature = current->feature;
			if (feature == "Sepal Length") {
				if (sepal_length[i] < threshhold) {
					current = current->below;
				} else {
					current = current->above;
				}	
			} else if (feature == "Sepal Width") {
				if (sepal_width[i] < threshhold) {
                                        current = current->below;
                                } else {
                                        current = current->above;
                                }
			} else if (feature == "Petal Length") {
				if (petal_length[i] < threshhold) {
                                        current = current->below;
                                } else {
                                        current = current->above;
                                }
			} else {
				if (petal_width[i] < threshhold) {
                                        current = current->below;
                                } else {
                                        current = current->above;
                                }
			}
		}
		//------------------------------------------------------------------
		//cout << "current guess: " << current->current_guess << endl;
		//cout << "Actual class: " << classification[i] << endl;
		//-----------------------------------------------------------------
		// Confirm the result
		if (current->current_guess == classification[i]) {
			correct++;
		}
	}
	//---------------------------------------------------------------------------------------
	//cout << "correct: " << correct << endl;
	//cout << "data_size: " << data_size << endl;
	//---------------------------------------------------------------------------------------
	// Return the probability
	double r = ((double)correct/(double)data_size);
	return r;	
}

// Function that find the maximum depth of the given tree
// Node* -> INT
int FindMaximumDepth(Node *root) {
	// Return the depth of 1 if this is the leaf
	if ((root->below == NULL) && (root->above == NULL)) {
		return 1;
	}
	// Otherwise. get the depth of teh dub tree and return the larger one
	int right = FindMaximumDepth(root->below);
	int left = FindMaximumDepth(root->above);
	if (right >= left) {
		return right+1;
	} else {
		return left+1;
	}
}

// Function that perform one iteration
// VECTOR<VECTOR<DOUBLE>> DOUBLE -> VECTOR<VECTOR<DOUBLE> >
vector<vector<double> > Update_Board(vector<vector<double> > board, double rs) {
	// New board to return
	vector<vector<double> > r;
	for (int i = 0; i < board.size(); i++) {
		vector<double> inner;
		for (int j = 0; j < board[i].size(); j++) {
			double up;
			double right;
			double down;
			double left;
			if ((i == 1) && (j == 1)) {
				inner.push_back(-7.0);
			} else if ((i == 1) && (j == 3)) {
				inner.push_back(-1.0);
			} else if ((i == 2) && (j == 3)) {
				inner.push_back(1.0);
			} else {
				if ((i == 0) && (i == board.size()-1)) {
					up = board[i][j];
					down = board[i][j];
				} else if (i == board.size()-1) {
					down = board[i][j];
					if (board[i-1][j] == -7.0) {
						up = board[i][j];
					} else {
						up = board[i-1][j];
					}
				} else if (i == 0) {
					up = board[i][j];
					if (board[i+1][j] == -7.0) {
						down = board[i][j];
					} else {
						down = board[i+1][j];
					}
				} else {
					if (board[i-1][j] == -7.0) {
                                                up = board[i][j];
                                        } else {
                                                up = board[i-1][j];
                                        }
					if (board[i+1][j] == -7.0) {
                                                down = board[i][j];
                                        } else {
                                                down = board[i+1][j];
                                        }
				}
				if ((j == 0) && (j == board[i].size()-1)) {
					left = board[i][j];
					right = board[i][j];
				} else if (j == board[i].size()-1) {
					right = board[i][j];
					if (board[i][j-1] == -7.0) {
						left = board[i][j];
					} else {
						left = board[i][j-1];
					}
				} else if (j ==0) {
					left = board[i][j];
					if (board[i][j+1] == -7.0) {
						right = board[i][j];
					} else {
						right = board[i][j+1];
					}
				} else {
					if (board[i][j-1] == -7.0) {
                                                left = board[i][j];
                                        } else {
                                                left = board[i][j-1];
                                        }
					if (board[i][j+1] == -7.0) {
                                                right = board[i][j];
                                        } else {
                                                right = board[i][j+1];
                                        }
				}
				double a = ((0.8*up) + (0.1*right) + (0.1*left));
				double b = ((0.8*right) + (0.1*down) + (0.1*up));
				double c = ((0.8*down) + (0.1*right) + (0.1*left));
				double d = ((0.8*left) + (0.1*down) + (0.1*up));
				if (((a >= b) && (a >= c)) && (a >= d)) {
					inner.push_back(a+rs);
				} else if (((b >= a) && (b >= c)) && (b >= d)) {
					inner.push_back(b+rs);
				} else if (((c >= a) && (c >= b)) && (c >= d)) {
					inner.push_back(c+rs);
				} else {
					inner.push_back(d+rs);
				}
			}
		}
		r.push_back(inner);	
	}
	return r;
}

// Function to print the board
// VECTOR<VECTOR<DOUBLE>> -> VOID
void Print_Board(vector<vector<double> > board) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << board[i][j] << "   ";
		}
		cout << endl;
	}
} 
	

int main() {

	cout << endl;
	cout << "Q1.1---------------------------------------------------------------------------------------------------------------------" << endl;
	// Cost for each step
	double rs = -0.05;
	// The whole board
	vector<vector<double> > board;
	// Initialize the board
	for (int b_idx = 0; b_idx < 3; b_idx++) {
		vector<double> inner;
		for (int inner_idx = 0; inner_idx < 4; inner_idx++) {
			if ((inner_idx == 1) && (b_idx == 1)) {
				inner.push_back(-7.0);
			} else if ((inner_idx == 3) && ((b_idx == 1) || (b_idx == 2))) {
				if (b_idx == 1) {
					inner.push_back(-1.0);
				} else {
					inner.push_back(1.0);
				}
			} else {
				inner.push_back(0.0);
			}
		}
		board.push_back(inner);
	}
	// Do 20 iteration with the given rs and current board
	for (int iterate = 0; iterate < 20; iterate++) {
		board = Update_Board(board,rs);
		cout << "Priting out U" << iterate+1 << endl;
		Print_Board(board);
		cout << endl;
	}
	
	cout << "Q1.2---------------------------------------------------------------------------------------------------------------------" << endl;
	// Cost for each step
        rs = -0.1;
        // The whole board
        vector<vector<double> > board2;
        // Initialize the board
        for (int b_idx = 0; b_idx < 3; b_idx++) {
                vector<double> inner;
                for (int inner_idx = 0; inner_idx < 4; inner_idx++) {
                	if ((inner_idx == 1) && (b_idx == 1)) {
                                inner.push_back(-7.0);
                        } else if ((inner_idx == 3) && ((b_idx == 1) || (b_idx == 2))) {
                                if (b_idx == 1) {
                                        inner.push_back(-1.0);
                                } else {
                                        inner.push_back(1.0);
                                }
                        } else {
                                inner.push_back(0.0);
                        }
                }
                board2.push_back(inner);
        }
        // Do 20 iteration with the given rs and current board
        for (int iterate = 0; iterate < 20; iterate++) {
                board2 = Update_Board(board2,rs);
                cout << "Priting out U" << iterate+1 << endl;
                Print_Board(board2);
		cout << endl;
        }
	

	// Read the file
	ifstream file("set_a.csv");
	// By lines
	string line;
	// vector for the sepal length
	vector<double> sepal_length;
	// vector for the sepal width
	vector<double> sepal_width;
	// vector for the petal length
	vector<double> petal_length;
	// vector for the petal width
	vector<double> petal_width;
	// vector for the classification
        vector<int> classification;
	// Read the file by line and parse data into each vector
	while (getline(file,line)) {
		// split the line by comma and put the first one into sepal_length
		string s = line;
		//------------------------------------------------
		//cout << "Parsing the string: " << s << endl;
		//------------------------------------------------
		size_t pos = s.find(",");
		string sub_s = s.substr(0,pos);
		sepal_length.push_back(atof(sub_s.c_str()));
		//-------------------------------------------------
		//cout << atof(sub_s.c_str()) << " added to sepal_length" << endl;
		//------------------------------------------------
		s.erase(0, pos + 1);
		// Put the second one into sepal_width
		pos = s.find(",");
		sub_s = s.substr(0,pos);
		sepal_width.push_back(atof(sub_s.c_str()));
		//-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to sepal_width" << endl;
                //------------------------------------------------
		s.erase(0, pos + 1);
		// Put the third one into petal_length
		pos = s.find(",");
                sub_s = s.substr(0,pos);
                petal_length.push_back(atof(sub_s.c_str()));
                //-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to petal_length" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
		// Put the fourth one into petal_width
		pos = s.find(",");
                sub_s = s.substr(0,pos);
                petal_width.push_back(atof(sub_s.c_str()));
                //-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to petal_width" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
		// Put the last one into classification
		classification.push_back(atoi(s.c_str()));
		//--------------------------------------------------------------
                //cout << atoi(s.c_str()) << " added to calssification" << endl;
                //--------------------------------------------------------------
	}

	// The root of the decision tree
	Node *root = MakeDecisionTree(sepal_length, sepal_width, petal_length, petal_width, classification, -1);


	// Print decision tree
	cout << "Q2.1---------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "The following is the decision tree. every node is printed as feature tested, threshhold to test, and the best guess of the class do far. If there are tie between best guesses (same in number sample), the class is picked randomly from the best guesses. Each indention is a new level of the tree. Left node (below threshhold) is always printed first then right node (above threshold)" << endl;	
	PrintDecisionTree(root, "");
	double accuracy = GetAccuracy(sepal_length, sepal_width, petal_length, petal_width, classification, root);
	cout << "The accurary of the decision tree is: " << accuracy << endl;


	// 10-fold cross validation
	cout << endl;
	cout << "Q2.2---------------------------------------------------------------------------------------------------------------------" << endl;
	int maximum_depth = FindMaximumDepth(root);	
	// Find the best accuracy for each depth to the maximum depth
	double best_accuracy = 0;
	int best_depth = 0;
	for (int current_depth = 1; current_depth <= maximum_depth; current_depth++) {
		// 10-fold
		double total_traning_accuracy = 0;
		double total_validate_accuracy = 0;
		for (int fold = 0; fold < 10; fold++) {
			// The starting index of the validate set
			int validate = fold*10;
			// Get the validate set
			vector<double> sepal_length_validate(sepal_length.begin()+validate,sepal_length.begin()+validate+9);
			vector<double> sepal_width_validate(sepal_width.begin()+validate,sepal_width.begin()+validate+9);
			vector<double> petal_length_validate(petal_length.begin()+validate,petal_length.begin()+validate+9);
                        vector<double> petal_width_validate(petal_width.begin()+validate,petal_width.begin()+validate+9);
			vector<int> classification_validate(classification.begin()+validate,classification.begin()+validate+9);
			// Get the training set
			vector<double> sepal_length_training = sepal_length;
                        vector<double> sepal_width_training = sepal_width;
                        vector<double> petal_length_training = petal_length;
                        vector<double> petal_width_training = petal_width;
                        vector<int> classification_traning = classification;
			for (int d = 0; d < 10; d++) {
				sepal_length_training.erase(sepal_length_training.begin()+validate);
				sepal_width_training.erase(sepal_width_training.begin()+validate);
				petal_length_training.erase(petal_length_training.begin()+validate);
				petal_width_training.erase(petal_width_training.begin()+validate);
				classification_traning.erase(classification_traning.begin()+validate);
			}
			// Make decision tree with current depth and training set
			Node *decision_tree = MakeDecisionTree(sepal_length_training,sepal_width_training,petal_length_training,petal_width_training,classification_traning,current_depth);
			// Add the training accuracy
			total_traning_accuracy = total_traning_accuracy + GetAccuracy(sepal_length_training,sepal_width_training,petal_length_training,petal_width_training,classification_traning,decision_tree);
			// Add the validate accurary
			total_validate_accuracy = total_validate_accuracy + GetAccuracy(sepal_length_validate,sepal_width_validate,petal_length_validate,petal_width_validate,classification_validate,decision_tree);
			DeleteDecisionTree(decision_tree);
		}
		double average_traning_accuracy = total_traning_accuracy/10.0;
		double average_validate_accuracy = total_validate_accuracy/10.0;
		cout << "The average prediction accuracy on the training data for maximum depth " << current_depth << " is: " << average_traning_accuracy << endl;
		cout << "The average prediction accuracy on the validation data for maximum depth " << current_depth << " is: " << average_validate_accuracy << endl;
		// Update the best so far
		if (best_accuracy < average_validate_accuracy) {
			best_accuracy = average_validate_accuracy;
			best_depth = current_depth;
		}
	}
	// Generate the tree on full data set with best depth
	Node *root_maximum_depth = MakeDecisionTree(sepal_length, sepal_width, petal_length, petal_width, classification, best_depth);
	cout << "The following is the decision tree. every node is printed as feature tested, threshhold to test, and the best guess of the class do far. If there are tie between best guesses (same in number sample), the class is picked randomly from the best guesses. Each indention is a new level of the tree. Left node (below threshhold) is always printed first then right node (above threshold)" << endl;
        PrintDecisionTree(root_maximum_depth, "");
        accuracy = GetAccuracy(sepal_length, sepal_width, petal_length, petal_width, classification, root_maximum_depth);
        cout << "The prediction accurary of the decision tree with the best maximum depth is: " << accuracy << endl;


	// Now test against other sata set
	cout << endl;
        cout << "Q2.3---------------------------------------------------------------------------------------------------------------------" << endl;	
	cout << "Now we check the accuracy against other data set. Please enter the name of the file to test against. Otherwise, please neter N and then the file named set_b.csv will be read by default" << endl;
	string file_name;
	cin >> file_name;
	if (file_name == "N") {
		file_name = "set_b.csv";
	}	
	ifstream fin(file_name.c_str());
        // By lines
        line;
        // vector for the sepal length
        sepal_length.clear();
        // vector for the sepal width
        sepal_width.clear();
        // vector for the petal length
        petal_length.clear();
        // vector for the petal width
        petal_width.clear();
        // vector for the classification
        classification.clear();
        // Read the file by line and parse data into each vector
        while (getline(fin,line)) {
                // split the line by comma and put the first one into sepal_length
                string s = line;
                //------------------------------------------------
                //cout << "Parsing the string: " << s << endl;
                //------------------------------------------------
                size_t pos = s.find(",");
                string sub_s = s.substr(0,pos);
                sepal_length.push_back(atof(sub_s.c_str()));
                //-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to sepal_length" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
                // Put the second one into sepal_width
                pos = s.find(",");
                sub_s = s.substr(0,pos);
                sepal_width.push_back(atof(sub_s.c_str()));
                //-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to sepal_width" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
                // Put the third one into petal_length
                pos = s.find(",");
                sub_s = s.substr(0,pos);
                petal_length.push_back(atof(sub_s.c_str()));
		//-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to petal_length" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
                // Put the fourth one into petal_width
                pos = s.find(",");
                sub_s = s.substr(0,pos);
                petal_width.push_back(atof(sub_s.c_str()));
                //-------------------------------------------------
                //cout << atof(sub_s.c_str()) << " added to petal_width" << endl;
                //------------------------------------------------
                s.erase(0, pos + 1);
                // Put the last one into classification
                classification.push_back(atoi(s.c_str()));
                //--------------------------------------------------------------
                //cout << atoi(s.c_str()) << " added to calssification" << endl;
                //--------------------------------------------------------------
        }
	accuracy = GetAccuracy(sepal_length, sepal_width, petal_length, petal_width, classification, root_maximum_depth);
        cout << "The accurary of the decision tree with the best maximum depth on other data set is: " << accuracy << endl;
	
	// Delete the decision tree
        DeleteDecisionTree(root);
        DeleteDecisionTree(root_maximum_depth);

	return 0;
}
