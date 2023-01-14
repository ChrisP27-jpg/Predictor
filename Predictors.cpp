#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
struct instruction{
	unsigned long long addr;
	string behavior;
	unsigned long long target;
};

int main(int argc, char *argv[]) {

  // Temporary variables
  unsigned long long addr;
  string behavior;
  unsigned long long target;
  int index;
  int first = 0;
  int second = 0;
  int three = 0;
  int four=0;
  int five = 0;
  int six = 0;
  int lines = 0;
  int tran=0;
  string previous = "T";
  string temp2;
  int x[] = {16, 32, 128, 256, 512, 1024, 2048};
  string y[]={"NN","NT" , "T" , "TT"};

  ifstream infile(argv[1]);
  ofstream fout(argv[2]);
  
  vector<instruction> file;
  int numInst=0;
  instruction temp;

  while(infile >> std::hex >> addr >> behavior >> std::hex >> target) {
    temp.addr = addr;
    temp.behavior=behavior;
    temp.target=target;
    file.push_back(temp);
    numInst++;
  }
  for (int i=0; i<numInst; i++){
  	 if(file.at(i).behavior == "T") {
      first++;
    }else {
      second++;
    }
  }
  
  fout << first << "," << numInst << ";" << endl;
  fout << second << "," << numInst << ";" << endl;


  for (int j=0; j < 7; j++){
  	 three = 0;
    string predict[x[j]];
	 for (int i =0; i <x[j]; i++){predict[i]="T";}

	 for(int i = 0; i < numInst; i++){
		
		index = file.at(i).addr % x[j];
		if(predict[index] == file[i].behavior){
			three++;
		}
		else{
			predict[index] = file[i].behavior;
		}
		
	 }
	 fout << three << "," << numInst << "; "; 
  }
  fout << endl;

  for (int j=0; j < 7; j++){
    four = 0;
    string predict[x[j]];
    for (int i =0; i <x[j]; i++){predict[i]="TT";}

    for(int i = 0; i < numInst; i++){
		
		index = file.at(i).addr % x[j];
		temp2=predict[index];
		if (predict[index]=="TT"){temp2 ="T";}
		if (predict[index]=="NN"){temp2 ="NT";}
		if(temp2 == file.at(i).behavior){
			four++;
		}
		
		if(file[i].behavior == "T"){
			if(predict[index] != "TT"){
				for (int l=0; l<4;l++){
					if (y[l]==predict[index]){predict[index] = y[l+1];l=4;}
				}
			}
		}
		else{
		  if(predict[index] != "NN"){
			 for (int l=0; l<4;l++){
				if (y[l]==predict[index]){predict[index] = y[l-1];l=4;}
			 }
		  }
		}
	}
	fout << four << "," << numInst << "; "; 
}

	fout << endl;
	
	for (int j =3; j<12; j++){
		five =0;
		int global = 0;
		string predict[2048];
		int globalSize = (int) pow(2, j) - 1;
		
		for (int i =0; i <2048; i++){predict[i]="TT";}
		
		for(int i = 0; i < numInst; i++){
			int index = (file[i].addr ^ (global & globalSize)) %2048;
			
			temp2=predict[index];
			if (predict[index]=="TT"){temp2 ="T";}
			if (predict[index]=="NN"){temp2 ="NT";}
			if(temp2 == file.at(i).behavior){
			five++;
		}
		
		if(file[i].behavior == "T"){
			if(predict[index] != "TT"){
				for (int l=0; l<4;l++){
					if (y[l]==predict[index]){predict[index] = y[l+1];l=4;}
				}
			}
		}
		else{
		  if(predict[index] != "NN"){
			 for (int l=0; l<4;l++){
				if (y[l]==predict[index]){predict[index] = y[l-1];l=4;}
			 }
		  }
		}
			global <<= 1;
			if (file[i].behavior == "T"){tran = 1;}
			else{tran = 0;}
			global |= tran;
		}
		fout << five << "," << numInst << "; "; 
	}
		
	fout << endl;
	
	
	int globalSize = (int) pow(2, 11) -1;
	int global = 0;
	string predictG[2048];
	string predictB[2048];
	string predictT[2048];
	
	for (int i=0; i <2048; i++){predictG[i]="TT";}
	for (int i=0; i <2048; i++){predictB[i]="TT";}
	for (int i=0; i <2048; i++){predictT[i]="TT";}
	
	for(int i = 0; i < numInst; i++){
		int tIndex = file[i].addr % 2048;
		int bIndex = file[i].addr % 2048;
		int gIndex = (file[i].addr ^ (global & globalSize)) % 2048;
		string bState = predictB[bIndex];	
		string gState = predictG[gIndex];
		string tState = predictT[tIndex];
		
		string temp3=predictB[bIndex];
		string temp4=predictG[gIndex];
		if (predictB[bIndex]=="TT"){temp3 ="T";}
		if (predictG[gIndex]=="NN"){temp4 ="NT";}
		
		if(file[i].behavior == "T"){
			if(gState != "TT")
			  for (int l=0; l<4;l++){
				 if (y[l]==gState){gState = y[l+1];l=4;}
			  }
			if(bState != "TT")
			  for (int l=0; l<4;l++){
				 if (y[l]==bState){bState = y[l+1];l=4;}
			  }
		}else{
			if(gState != "NN")
			  for (int l=0; l<4;l++){
				 if (y[l]==gState){gState = y[l-1];l=4;}
			  }
			if(bState != "NN")
			  for (int l=0; l<4;l++){
				 if (y[l]==bState){bState = y[l-1];l=4;}
			  }
		}
		
		global <<= 1;
		if (file[i].behavior == "T"){tran = 1;}
		else{tran = 0;}
		global |= tran;
		predictG[gIndex] = gState;
		predictB[bIndex] = bState;
		
		if(temp3 == temp4){
			if(file[i].behavior == temp4){
				six++;
			}
		}
	
		else{
			if(tState == "TT" || tState == "T"){
				if(file[i].behavior == temp4){
					six++;
					
					if(tState != "TT")
						for (int l=0; l<4;l++){
				 			if (y[l]==tState){tState = y[l+1];l=4;}
			  			}
				}
				else{
					if(tState != "NN")
						for (int l=0; l<4;l++){
				 			if (y[l]==tState){tState = y[l-1];l=4;}
			  			}
				}
			}
			
			else{
				
				if(file[i].behavior == temp3){
					six++;
					if(tState != "NN")
						for (int l=0; l<4;l++){
				 			if (y[l]==tState){tState = y[l-1];l=4;}
			  			}
				}
				
				else{
					if(tState != "TT")
						for (int l=0; l<4;l++){
				 			if (y[l]==tState){tState = y[l+1];l=4;}
			  			}
				}
			}
		}
		predictT[tIndex] = tState;
	}
  
  fout << six << "," << numInst << "; "; 
  fout << endl;

  infile.close();
  fout.close();
  return 0;
}