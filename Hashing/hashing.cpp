#include <bits/stdc++.h>
#include <fstream>

using namespace std;

//HelperFunctions
bool checkPrime(int value){
    for(int i=2; i<=ceil(sqrt(value)); i++)
        if(value%i==0)
            return false;
    return true;
}

int getActualSizeOfTable(int minSizeOfTable){
    int sizeOfTable = minSizeOfTable;
    while(!checkPrime(sizeOfTable))
        sizeOfTable++;
    return sizeOfTable;
}

//All Hash Functions
class HashFunctions{
    int sizeOfTable;
public:
    HashFunctions() {}
    HashFunctions(int S){
        sizeOfTable = S;
    }

    void setSizeOfTable(int S){
        sizeOfTable = S;
    }

    int HashDivision(int key){
        int modValueForDivision = 5;
        return key%modValueForDivision;
    }

    int HashMidSquare(int key){
        int numberOfMidDigits = ceil(log10(sizeOfTable));

        int squareOfKey = key*key;
        int lenOfSquare = ceil(log10(squareOfKey));
        int midEnd = ( lenOfSquare/2 ) + 1;
        midEnd = lenOfSquare-midEnd;

        midEnd = ceil(pow(10, midEnd));
        squareOfKey = squareOfKey/midEnd;
        numberOfMidDigits = ceil(pow(10, numberOfMidDigits));

        return squareOfKey%numberOfMidDigits;
    }

    int HashDigitFold(int key){
        int lenOfKey = ceil(log10(key));
        
        int numberOfStartDigits = ceil(log10(sizeOfTable));
        int divider = ceil(pow(10, lenOfKey-numberOfStartDigits));

        int hashValue = 0;
        while(divider){
            hashValue += key/divider;
            key = key%divider;
            divider = divider/ceil(pow(10,numberOfStartDigits));
        }hashValue+=key;

        return hashValue;
    }

    int HashMuliplication(int key){
        double multiplicationConst = .05;
        double fullPart = key*multiplicationConst;
        int intPart = fullPart;
        double fractionPart = fullPart - intPart;
        return floor( sizeOfTable * fractionPart);
    }

    int HashPolynomialString(string str){
        int xconstant = 31;
        long long int powerOfxConstant = 1;
        int hashValue = 0;

        for(char c: str){
            hashValue = ( hashValue + (c-'a'+1)*powerOfxConstant ) % sizeOfTable;
            powerOfxConstant = (powerOfxConstant*xconstant) % sizeOfTable;
        }

        return hashValue;
    }

    int HashPolynomialString2(string str){
        int xconstant = 7;
        long long int powerOfxConstant = 1;
        int hashValue = 0;

        for(char c: str){
            hashValue >>= 2;
            hashValue = ( hashValue + (c-'a'+1)*powerOfxConstant ) % sizeOfTable;
            powerOfxConstant = (powerOfxConstant*xconstant) % sizeOfTable;
        }

        return hashValue;
    }

    int HashPolynomialStringaux(string str){
        int xconstant = 37;
        long long int powerOfxConstant = 1;
        int hashValue = 0;

        for(char c: str){
            hashValue >>= 4;
            hashValue = ( hashValue + (c-'a'+1)*powerOfxConstant ) % sizeOfTable;
            powerOfxConstant = (powerOfxConstant*xconstant) % sizeOfTable;
        }

        return hashValue;
    }
};

class randomWordGenerator{
    int numberOfRandomWords;
    int minWordLength, maxWordLength;
    vector<string> randomWords;

public:
    randomWordGenerator() {}
    randomWordGenerator(int N){
        numberOfRandomWords = N;
    }
    randomWordGenerator(int N, int min, int max){
        numberOfRandomWords = N;
        minWordLength = min; maxWordLength = max;
        randomWordGeneration(numberOfRandomWords);
    }
    void setWordLength(int min, int max){
        minWordLength = min; maxWordLength = max;
    }
    vector<string> getRandomWords(){ return randomWords; }

    //RandomWordGenerator
    void randomWordGeneration(int N){
        numberOfRandomWords = N;
        int diff = maxWordLength-minWordLength;

        srand(15);

        for(int i=0; i<numberOfRandomWords; i++){
            int randomLength = minWordLength+rand()%diff;

            string randomWord;
            for(int j=0; j<randomLength; j++){
                char randomChar = 'a'+rand()%26;
                randomWord += randomChar;
            }

            randomWords.push_back(randomWord);
            randomWord.clear();
        }

        /*
        ofstream randomWordFile("randomWordFile.txt");
        for(auto i: randomWords) randomWordFile<<i<<endl;
        randomWordFile.close();
        */
    }
};

class separateChainHashing{
    vector<list<string>> separateChainHashTable;
    int sizeOfTable;

    int totalCollsion;
    vector<int> numberOfCollisions;

    HashFunctions HF;
    int hashFunctionCode;

    int numberOfRandomWords;
    vector<string> randomWords;

public:
    separateChainHashing() {}
    separateChainHashing(int minSizeOfTable, int N, int hfc){
        numberOfRandomWords = N;
        sizeOfTable = getActualSizeOfTable(minSizeOfTable);

        separateChainHashTable.resize(sizeOfTable);
        HF.setSizeOfTable(sizeOfTable);
        hashFunctionCode = hfc;

        numberOfCollisions.resize(sizeOfTable);
        for(auto i: numberOfCollisions) i = 0;
        totalCollsion = 0;
    }

    void setRandomWords(vector<string> W){ randomWords = W; }
    int getNumberOfCollisions() { return totalCollsion; }
    float getCollPer() { return (float)totalCollsion/numberOfRandomWords*100; }
    int getSizeOfTable() { return sizeOfTable; }
    vector<string> getRandomWords() { return randomWords; }
    vector<int> getListOfCollisions() { return numberOfCollisions; }

    void HashAll(){
        for(int i=0; i<numberOfRandomWords; i++)
            Insert(randomWords[i]);
    }//Insert Randomly Generated Words

    bool Insert(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        //Ignore insertion in case of repeated value
        for(auto j: separateChainHashTable[hashValue])
            if(j==str) return false;

        //Keeping count of collision
        if(!separateChainHashTable[hashValue].empty()){
            totalCollsion++;
            numberOfCollisions[hashValue]++;
        }

        //If unique string, we insert value
        separateChainHashTable[hashValue].push_back(str);
        //cout<<i<<"  "<<"Reached Here"<<endl;
        return true;
    }//Insert Function

    pair<bool, int> Find(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        if(separateChainHashTable[hashValue].empty()) return make_pair(false, 0);
        else{
            for(auto j: separateChainHashTable[hashValue])
                if(j==str) return make_pair(true, hashValue);
        }

        return make_pair(false, 0);
    }//Find Function

    bool Delete(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        if(separateChainHashTable[hashValue].empty()) return false;
        for(auto i: separateChainHashTable[hashValue]){
            if(i==str){
                separateChainHashTable[hashValue].remove(str);
                return true;
            }
        }

        return false;
    }//Delete Function

    void Operations(){
        char ops = 'a';
        string str;
        while(ops != 'q'){
            cin>>ops;  cin>>str;
            if(ops == 'f') cout<<Find(str).first<<endl;
            if(ops == 'd') cout<<Delete(str)<<endl;
        }
    }//Operations Function
};

class doubleHashing{
    vector<string> hashTable;
    int sizeOfTable;

    vector<int> numberOfCollisions;
    int totalCollisions;

    HashFunctions HF;
    int hashFunctionCode;

    int numberOfRandomWords;
    vector<string> randomWords;
public:
    doubleHashing() {}
    doubleHashing(int minSizeOfTable, int N, int hfc){
        numberOfRandomWords = N;
        sizeOfTable = getActualSizeOfTable(minSizeOfTable);
        
        hashTable.resize(sizeOfTable);
        HF.setSizeOfTable(sizeOfTable);
        hashFunctionCode = hfc;

        numberOfCollisions.resize(sizeOfTable);
        for(auto i: numberOfCollisions) i = 0;
        totalCollisions = 0;
    }

    void setRandomWords(vector<string> W){ randomWords = W; }
    int getNumberOfCollisions(){
        totalCollisions = 0;
        for(auto i: numberOfCollisions) totalCollisions += i;
        return totalCollisions;
    }
    float getCollPer() { return (float)totalCollisions/numberOfRandomWords*100; }
    int maxCollision() {
        int maxCol = 0;
        for(auto i: numberOfCollisions)
            maxCol = max(maxCol, i);
        return maxCol;
    }
    int getSizeOfTable() { return sizeOfTable; }
    vector<string> getRandomWords() { return randomWords; }
    vector<int> getListOfCollisions() { return numberOfCollisions; }

    void HashAll(){
        for(int i=0; i<numberOfRandomWords; i++)
            Insert(randomWords[i]);
    }//Insert Randomly Generated Words

    bool Insert(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        //Ignore insertion in case of repeated value
        if(!hashTable[hashValue].empty()) numberOfCollisions[hashValue]++;
        if(hashTable[hashValue] == str) return false;

        //->Check for Collision
        //No Collision->Primary Probe
        if(numberOfCollisions[hashValue]==0){
            hashTable[hashValue] = str;
            return true;
        }
        //Collision detected and Double Hashing Executed->Successive Probes
        else{
            hashValue = ( hashValue + ( numberOfCollisions[hashValue]*HF.HashPolynomialStringaux(str) ) ) % sizeOfTable;

            if(hashTable[hashValue].empty()){
                hashTable[hashValue] = str;
                return true;
            }

            else return false; //else, we ignore the insertion
        }
    }//Insertion Function

    pair<bool, int> Find(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        //Primary Probe
        if(hashTable[hashValue].empty()) return make_pair(false, 0);
        else if(hashTable[hashValue] == str) return make_pair(true, hashValue);
        
        //Successive Probes
        int maxCol = maxCollision();
        for(int i = 1; i <= maxCol; i++){
            hashValue = ( hashValue + ( i*HF.HashPolynomialStringaux(str) ) ) % sizeOfTable;
            if(hashTable[hashValue] == str) return make_pair(true, hashValue);
        }

        return make_pair(false, 0);
    }//Find Function

    bool Delete(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);

        //Primary Probe
        if(hashTable[hashValue].empty()) return false;
        else if(hashTable[hashValue] == str){
            hashTable[hashValue].clear();
            return true;
        }
        
        //Successive Probes
        int maxCol = maxCollision();
        for(int i = 1; i <= maxCol; i++){
            hashValue = ( hashValue + ( i*HF.HashPolynomialStringaux(str) ) ) % sizeOfTable;
            if(hashTable[hashValue] == str){
                hashTable[hashValue].clear();
                return true;
            }
        }

        return false;
    }//Delete Function

    void Operations(){
        char ops = 'a';
        string str;

        while(ops != 'q'){
            cin>>ops;  cin>>str;
            if(ops == 'f') cout<<Find(str).first<<endl;
            if(ops == 'd') cout<<Delete(str)<<endl;
        }
    }//Operations Function
};

class customHash{
    vector<string> hashTable;
    int sizeOfTable;

    vector<int> numberOfCollisions;
    int totalCollisions;

    HashFunctions HF;
    int hashFunctionCode;

    int numberOfRandomWords;
    vector<string> randomWords;
public:
    customHash() {}
    customHash(int minSizeOfTable, int N, int hfc){
        numberOfRandomWords = N;
        sizeOfTable = getActualSizeOfTable(minSizeOfTable);
        
        hashTable.resize(sizeOfTable);
        HF.setSizeOfTable(sizeOfTable);
        hashFunctionCode = hfc;

        numberOfCollisions.resize(sizeOfTable);
        for(auto i: numberOfCollisions) i = 0;
        totalCollisions = 0;
    }

    void setRandomWords(vector<string> W){ randomWords = W; }
    int getNumberOfCollisions(){
        totalCollisions = 0;
        for(auto i: numberOfCollisions) totalCollisions += i;
        return totalCollisions;
    }
    float getCollPer() { return (float)totalCollisions/numberOfRandomWords*100; }
    int maxCollision() {
        int maxCol = 0;
        for(auto i: numberOfCollisions)
            maxCol = max(maxCol, i);
        return maxCol;
    }
    int getSizeOfTable() { return sizeOfTable; }
    vector<string> getRandomWords() { return randomWords; }
    vector<int> getListOfCollisions() { return numberOfCollisions; }

    void HashAll(){
        for(int i=0; i<numberOfRandomWords; i++)
            Insert(randomWords[i]);
    }//Insert Randomly Generated Words

    bool Insert(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);
        int xConstant1 = 2; int xConstant2 = 4;

        //Ignore insertion in case of repeated value
        if(!hashTable[hashValue].empty()) numberOfCollisions[hashValue]++;
        if(hashTable[hashValue] == str) return false;

        //->Check for Collision
        //No Collision->Primary Proble
        if(numberOfCollisions[hashValue]==0){
            hashTable[hashValue] = str;
            return true;
        }
        //Collision detected and Double Hashing Executed->Successsive Probes
        else{
            hashValue = ( hashValue + ( xConstant1*numberOfCollisions[hashValue]*HF.HashPolynomialStringaux(str) )
                                    + ( xConstant2*numberOfCollisions[hashValue]*numberOfCollisions[hashValue]) ) % sizeOfTable;

            if(hashTable[hashValue].empty()){
                hashTable[hashValue] = str;
                return true;
            }

            else return false; //else, we ignore the insertion
        }
    }//Insertion Function

    pair<bool, int> Find(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);
        int xConstant1 = 2; int xConstant2 = 4;

        if(hashTable[hashValue].empty()) return make_pair(false, 0);
        else if(hashTable[hashValue] == str) return make_pair(true, hashValue);
        
        int maxCol = maxCollision();
        for(int i = 1; i <= maxCol; i++){
            hashValue = ( hashValue + ( xConstant1*numberOfCollisions[hashValue]*HF.HashPolynomialStringaux(str) )
                                    + ( xConstant2*numberOfCollisions[hashValue]*numberOfCollisions[hashValue]) ) % sizeOfTable;
            if(hashTable[hashValue] == str) return make_pair(true, hashValue);
        }

        return make_pair(false, 0);
    }//Find Function

    bool Delete(string str){
        int hashValue;
        if(hashFunctionCode == 1) hashValue = HF.HashPolynomialString(str);
        else if(hashFunctionCode == 2) hashValue = HF.HashPolynomialString2(str);
        int xConstant1 = 2; int xConstant2 = 4;

        if(hashTable[hashValue].empty()) return false;
        else if(hashTable[hashValue] == str){
            hashTable[hashValue].clear();
            return true;
        }
        
        int maxCol = maxCollision();
        for(int i = 1; i <= maxCol; i++){
            hashValue = ( hashValue + ( xConstant1*numberOfCollisions[hashValue]*HF.HashPolynomialStringaux(str) )
                                    + ( xConstant2*numberOfCollisions[hashValue]*numberOfCollisions[hashValue]) ) % sizeOfTable;
            
            if(hashTable[hashValue] == str){
                hashTable[hashValue].clear();
                return true;
            }
        }

        return false;
    }//Delete Function

    void Operations(){
        char ops = 'a';
        string str;

        while(ops != 'q'){
            cin>>ops;  cin>>str;
            if(ops == 'f') cout<<Find(str).first<<endl;
            if(ops == 'd') cout<<Delete(str)<<endl;
        }
    }//Operations Function
};

class reportGenerator{
    separateChainHashing sch1;
    doubleHashing dh1;
    customHash ch1;

    separateChainHashing sch2;
    doubleHashing dh2;
    customHash ch2;

    int numberOfRandomWords;
    vector<string> randomWords;
    int sizeOfTable;

    vector<string> randomlySelectedWords;
    int numberOfRandomlySelectedWords;

    int separateChainProbe1;
    int doubleHashProbe1;
    int customHashProbe1;

    int separateChainProbe2;
    int doubleHashProbe2;
    int customHashProbe2;
public:
    reportGenerator(int N, separateChainHashing S1, doubleHashing D1, customHash C1, separateChainHashing S2, doubleHashing D2, customHash C2){
        numberOfRandomWords = N;
        sch1 = S1; dh1 = D1; ch1 = C1;
        sch2 = S2; dh2 = D2; ch2 = C2;

        randomWords = sch1.getRandomWords();
        numberOfRandomlySelectedWords = 1000;

        sizeOfTable = sch1.getSizeOfTable();
        randomSelectionOfWords();
    }

    void randomSelectionOfWords(){
        int count = 0;
        vector<bool> visited(numberOfRandomWords, false);

        srand(15);
        while(count < numberOfRandomlySelectedWords){
            int selectionNo = rand()%numberOfRandomWords;
            if(!visited[selectionNo]){
                randomlySelectedWords.push_back(randomWords[selectionNo]);
                visited[selectionNo] = true;
                count++;
            }
        }
        return;
    }

    void findProbes(){
        vector<int> SnumberOfCollisions;
        vector<int> DnumberOfCollisions;
        vector<int> CnumberOfCollisions;

        SnumberOfCollisions = sch1.getListOfCollisions();
        DnumberOfCollisions = dh1.getListOfCollisions();
        CnumberOfCollisions = ch1.getListOfCollisions();

        separateChainProbe1 = 0;
        doubleHashProbe1 = 0;
        customHashProbe1 = 0;

        //Search for words and counting total probes
        pair<bool, int> temp;
        for(auto word: randomlySelectedWords){
            temp = sch1.Find(word);
            if(temp.first) separateChainProbe1 += (SnumberOfCollisions[temp.second]+1);
            temp = dh1.Find(word);
            if(temp.first) doubleHashProbe1 += (DnumberOfCollisions[temp.second]+1);
            temp = ch1.Find(word);
            if(temp.first) customHashProbe1 += (CnumberOfCollisions[temp.second]+1);
        }

        SnumberOfCollisions = sch2.getListOfCollisions();
        DnumberOfCollisions = dh2.getListOfCollisions();
        CnumberOfCollisions = ch2.getListOfCollisions();

        separateChainProbe2 = 0;
        doubleHashProbe2 = 0;
        customHashProbe2 = 0;

        //Search for words and counting total probes
        for(auto word: randomlySelectedWords){
            temp = sch2.Find(word);
            if(temp.first) separateChainProbe2 += (SnumberOfCollisions[temp.second]+1);
            temp = dh2.Find(word);
            if(temp.first) doubleHashProbe2 += (DnumberOfCollisions[temp.second]+1);
            temp = ch2.Find(word);
            if(temp.first) customHashProbe2 += (CnumberOfCollisions[temp.second]+1);
        }
    }//Find Total Probes Function

    void reportGeneration(ofstream &reportStream){
        findProbes();

        float separateChainAvg1 = (float)separateChainProbe1/numberOfRandomlySelectedWords;
        float doubleHashAvg1 = (float)doubleHashProbe1/numberOfRandomlySelectedWords;
        float customHashAvg1 = (float)customHashProbe1/numberOfRandomlySelectedWords;
        
        float separateChainAvg2 = (float)separateChainProbe2/numberOfRandomlySelectedWords;
        float doubleHashAvg2 = (float)doubleHashProbe2/numberOfRandomlySelectedWords;
        float customHashAvg2 = (float)customHashProbe2/numberOfRandomlySelectedWords;

        reportStream<<endl<<endl;
        reportStream<<"                 |                              |                Hash1               |               Hash2"<<endl;
        reportStream<<" Hash Table Size |  Collision Resolution Method |____________________________________|___________________________________"<<endl;
        reportStream<<"                 |                              | #NumberOfCollsions | AverageProbes | #NumberOfCollsions | AverageProbes"<<endl;
        reportStream<<"_________________|______________________________|____________________________________|___________________________________"<<endl;
        reportStream<<endl;
        reportStream<<"   "<<sizeOfTable<<"                   Chaining                       "<<sch1.getNumberOfCollisions()<<"             "<<separateChainAvg1<<"                "<<sch2.getNumberOfCollisions()<<"             "<<separateChainAvg2<<"          "<<endl;
        reportStream<<endl;
        reportStream<<"   "<<sizeOfTable<<"                Double Hashing                    "<<dh1.getNumberOfCollisions()<<"             "<<doubleHashAvg1<<"                "<<dh2.getNumberOfCollisions()<<"             "<<doubleHashAvg2<<"          "<<endl;
        reportStream<<endl;
        reportStream<<"   "<<sizeOfTable<<"                Custom Probing                    "<<ch1.getNumberOfCollisions()<<"             "<<customHashAvg1<<"                "<<ch2.getNumberOfCollisions()<<"             "<<customHashAvg2<<"          "<<endl;
        reportStream<<endl;
        reportStream<<"_________________|______________________________|____________________________________|___________________________________"<<endl;

        return;
    }
};

int main(void){
    int minSizeOfTable, numberOfRandomWords;
    minSizeOfTable = 5000; numberOfRandomWords = 10000;
    vector<int> minSizesOfTable = {5000, 10000, 20000};
    /*//Custom Table Size
    cout<<"Minimum size of table: "; cin>>minSizeOfTable;
    cout<<endl<<"Number of Random Words: "; cin>>numberOfRandomWords;
    cout<<endl;
    */

    //FunctionCodes
    vector<int> hashFunctionCodes = {1, 2};

    //Random Word Generation
    int minWordLength = 5, maxWordLenghth = 10;
    //cin>>minWordLength>>maxWordLenghth;
    randomWordGenerator rwg(numberOfRandomWords, minWordLength, maxWordLenghth);
    vector<string> randomWords = rwg.getRandomWords();

    //Report File
    ofstream reportStream("reportFile.txt");

    for(int sizeOfT = 0; sizeOfT<3; sizeOfT++){
        //Hash Table Declarations
        separateChainHashing sch1(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[0]);
        doubleHashing dh1(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[0]);
         customHash ch1(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[0]);

        separateChainHashing sch2(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[1]);
        doubleHashing dh2(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[1]);
        customHash ch2(minSizesOfTable[sizeOfT], numberOfRandomWords, hashFunctionCodes[1]);

        //Hash Table Executions
        sch1.setRandomWords(randomWords);
        sch1.HashAll();
        sch2.setRandomWords(randomWords);
        sch2.HashAll();
    
        dh1.setRandomWords(randomWords);
        dh1.HashAll();
        dh2.setRandomWords(randomWords);
        dh2.HashAll();

        ch1.setRandomWords(randomWords);
        ch1.HashAll();
        ch2.setRandomWords(randomWords);
        ch2.HashAll();

        //Report Generation 
        reportGenerator rg(numberOfRandomWords, sch1, dh1, ch1, sch2, dh2, ch2);
        rg.reportGeneration(reportStream);
    }
    
    reportStream.close();
    return 0;
}