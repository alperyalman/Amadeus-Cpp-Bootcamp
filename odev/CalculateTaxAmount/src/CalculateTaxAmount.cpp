#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

map<float, float> readFile(string fileName)
{
    map<float, float> taxInfo;
    string myText;
    float taxVal, taxPerc;
    ifstream MyReadFile(fileName);
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, myText))
    {
        // Output the text from the file
        stringstream ss(myText);
        ss >> taxVal >> taxPerc;
        taxInfo[taxVal] = taxPerc;
    }
    MyReadFile.close();

    return taxInfo;
}

void CalculateTaxAmount(float incomeValue, map<float, float> mapTaxInfo)
{

    float totalValue = 0;
    float taxAmount = 0;

    // Get an iterator pointing to the first element in the map
    auto it = mapTaxInfo.begin();

    // Iterate through the map
    while (it != mapTaxInfo.end())
    {

        if (incomeValue > it->first)
        {
            taxAmount = taxAmount + ((it->first - totalValue) * it->second) / 100;
            totalValue += (it->first - totalValue);
        }
        else
        {
            taxAmount = taxAmount + ((incomeValue - totalValue) * it->second) / 100;
            totalValue = incomeValue;
            break;
        }

        it++;
    }
    cout << "Total tax amount: " << taxAmount << endl;
}

int main()
{

    map<float, float> taxInfo;
    float incomeValue;
    taxInfo = readFile("TaxReferenceInfo.txt");

    cout << "Enter an income value: ";
    cin >> incomeValue;

    CalculateTaxAmount(incomeValue, taxInfo);

    return 0;
}