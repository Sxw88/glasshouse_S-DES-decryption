#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

//my attempt at a mock Simplified DES algorithm that is kind of transparent
//this one is the decryption algorithm

int str_to_int(std::string x) { ///converts string to integer
    int answer = 0;
    int power = x.length()-1;
    int i = 0;
    int dec = 10;
    int num = 0;

    while(power >= 0)
    {
        switch(x[i])
        {
            case '0':
            num = 0;
            break;

            case '1':
            num = 1;
            break;

            case '2':
            num = 2;
            break;

            case '3':
            num = 3;
            break;

            case '4':
            num = 4;
            break;

            case '5':
            num = 5;
            break;

            case '6':
            num = 6;
            break;

            case '7':
            num = 7;
            break;

            case '8':
            num = 8;
            break;

            case '9':
            num = 9;
            break;

            default:
            num = 0;
            std::cout << std::endl << "Data read is not in the form of integers. Input is bad." << std::endl;
            break;
        }

        for (int q = 1; q < power; q ++)
        {
            dec = dec*10;
        }
        if (power == 0)
        {
            dec = 1;
        }

        answer += num*dec;
        //std::cout << "value :" << x[i] << std::endl;
        //std::cout << "answer :" << answer << std::endl;
        power -= 1;
        i+=1;
        dec = 10;
    }
    return answer;
}

std::string rearrange(std::vector<int> px, std::string k_copy) {
    std::string k = "";

    //iterate thru the permutation array
    for (int i=0; i < px.size(); i++) {
        if (px[i] <= (k_copy.length()-1)/2) { //considering the space separating two parts of the key, 01234=12345; 678910=678910; 5=space
            k = k + k_copy[px[i]-1];
        }
        else {
            k = k + k_copy[px[i]];
        }

        //add a space at the middle of the key
        if (k.length() == px.size()/2) {
            k = k + " ";
        }
    }

    return k;
}

int minint(int i, int j) { ///compares numberes
    int l = 1;

    if (i < j)
    {
        l = i;
    }
    else
    {
        l = j;
    }

    return l;
}

std::string x_or(std::string str1, std::string str2) { ///xor
    //assuming 2 strings have the same length, but will take the one with less length
    std::string result = "";

    for (int i = 0; i < minint(str1.length(), str2.length()); i++) {
        if (str1[i] == str2[i]){
            result = result + "0";
        } else {
            result = result + "1";
        }
    }

    return result;
}

int BintoDec(std::string i) { ///converts binary string to decimal int
    int j = 0;
    int sz = i.length();
    int pwr = 0;

    while(sz > 0)
    {
        if(i[sz-1] == '1')
        {
            int p = pwr;
            int sum = 1;

            while(p > 0)
            {
                sum = sum*2;
                p--;
            }

            j += sum;
        }
        pwr ++;
        sz --;
    }

    return j;
}

void PrintVec(std::vector<std::vector<std::string>> vec) { ///print out a double vector, for debugging purposes
    std::ofstream ofvec;
    ofvec.open("log.txt", std::ios::out | std::ios::trunc);

    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec[i].size(); ++j) {
            ofvec << vec[i][j] << " ";
        }
        ofvec << std::endl;
    }
    ofvec.close();
    system("notepad log.txt");
}


int main()
{
    //temp string used for getting input and stuff
    std::string temp;
    //temporary vector for storing permutation key
    std::vector<int> px;
    //vector for keys
    std::vector<std::string> k;

    ///read the input file
        //open up the input file, for reading
        std::ifstream rfile_in;
        rfile_in.open("input.txt", std::ios::in);
        //get rid of first 2 lines of comments in the text file
        std::getline(rfile_in, temp); std::getline(rfile_in, temp);
        //read the 8-bit ciphertext and read the 10-bit key
        std::string ctxt; getline(rfile_in, ctxt);
        std::string key; getline(rfile_in, key);
        int rounds; getline(rfile_in, temp); rounds = str_to_int(temp);

    ///KEY GENERATION
    std::cout << "===========" << std::endl << "| KEY GEN |" << std::endl << "===========" << std::endl;
    std::cout << "KEY           : " << key << std::endl;

    //left shift file
    std::ifstream rfile_ls;

    //p8 file
    //open up the p_8 file
        std::ifstream rfile_p8;
        rfile_p8.open("./keygen/p_8.txt", std::ios::in);
        //get rid of first 3 lines of comments in text file
        std::getline(rfile_p8,temp); std::getline(rfile_p8,temp); std::getline(rfile_p8,temp);
        //get the permutation array in a vector
        std::vector<int> vec_p8;
        while (rfile_p8 >> temp) {
            vec_p8.push_back(str_to_int(temp));
        }
        rfile_p8.close();

    //run p10 on the key
        //open up the p_10 file
        std::ifstream rfile_p10;
        rfile_p10.open("./keygen/p_10.txt", std::ios::in);
        //get rid of first 3 lines of comments in text file
        std::getline(rfile_p10,temp); std::getline(rfile_p10,temp); std::getline(rfile_p10,temp);
        //get the permutation array into a vector
        while (rfile_p10 >> temp) {
            px.push_back(str_to_int(temp));
        }
        key = rearrange(px,key);
        rfile_p10.close(); px.clear();

        std::cout << "P10           : " << key << std::endl;

    //iterate based on number of rounds
    for (int x = 1; x < rounds+1; x++) {
        //run left shift x+1 on the key
            std::string ls_filename = "./keygen/ls_" + std::to_string(x%5) + ".txt";
            //open up the left shift file
            rfile_ls.open(ls_filename, std::ios::in);
            //get rid of the first 3 lines of comments in the text file
            std::getline(rfile_ls, temp); std::getline(rfile_ls, temp); std::getline(rfile_ls, temp);
            //get the permutation array in a vector
            while (rfile_ls >> temp) {
                px.push_back(str_to_int(temp));
            }
            key = rearrange(px,key);
            rfile_ls.close(); px.clear();

            std::cout << "Left-Shift " << x%5 << "  : " << key << std::endl;

        //acquire key x+1 by running p8 on the key
            //open up the p_8 file
            k.push_back(rearrange(vec_p8,key));

            //print key of each round
            if (x < 10) {std::cout << "Key " << x << "         :  " << k[x-1] << std::endl;}
            else {std::cout << "Key " << x << "        :  " << k[x-1] << std::endl;}
        }

    ///DECRYPTION
    std::string lf_txt = ""; //string to store left halve of the text

    std::vector<int> vec_ep; //vector for the 'expand and rearrange' operation
    std::vector<std::vector<std::string>> sbox0;
    std::vector<std::vector<std::string>> sbox1;
    std::vector<int> vec_p4;

    std::cout << std::endl << "==============" << std::endl << "| DECRYPTION |" << std::endl << "==============" << std::endl;
    std::cout << "CIPHERTEXT       : " << ctxt << std::endl;

    //initialize the expand and permutate vector (vec_ep)
        //open up the E/P file
        std::ifstream rfile_ep;
        rfile_ep.open("./encryp/EP.txt", std::ios::in);
        //get rid of the first 3 lines of comments in the text file
        std::getline(rfile_ep, temp); std::getline(rfile_ep, temp); std::getline(rfile_ep, temp);
        //get the permutation array in a vector
        while (rfile_ep >> temp) {
            vec_ep.push_back(str_to_int(temp)+4);
        }
        rfile_ep.close();

    //initialize sbox vectors (sbox0 and sbox1)
    //this part will need to be reworked to fit other sizes matrices
        //open up sbox files
        //sbox 0
        {std::ifstream rfile_s0;
        rfile_s0.open("./encryp/sbox0.txt", std::ios::in);
        //get rid of the first 3 lines of comments in the text file
        std::getline(rfile_s0, temp); std::getline(rfile_s0, temp); std::getline(rfile_s0, temp);
        //get the sbox contained in a vector
        while (!rfile_s0.eof()) {
            std::getline(rfile_s0, temp);
            std::string tempInt = "";
            std::vector<std::string> tempVec;

            for (int i = 0; i < temp.length(); i++) {
                char c = temp[i];

                if (c != ' ' && i != temp.length() -1) {
                    tempInt = tempInt + c;
                }
                else {
                    if (c != ' ') { tempInt = tempInt + c; }
                    tempVec.push_back(tempInt);
                    tempInt = "";
                }
            }
            sbox0.push_back(tempVec);
            tempVec.clear();
        }
        rfile_s0.close(); }
        //sbox1
        {std::ifstream rfile_s1;
        rfile_s1.open("./encryp/sbox1.txt", std::ios::in);
        //get rid of the first 3 lines of comments in the text file
        std::getline(rfile_s1, temp); std::getline(rfile_s1, temp); std::getline(rfile_s1, temp);
        //get the sbox contained in a vector
        while (!rfile_s1.eof()) {
            std::getline(rfile_s1, temp);
            std::string tempInt = "";
            std::vector<std::string> tempVec;

            for (int i = 0; i < temp.length(); i++) {
                char c = temp[i];

                if (c != ' ' && i != temp.length() -1) {
                    tempInt = tempInt + c;
                }
                else {
                    if (c != ' ') { tempInt = tempInt + c; }
                    tempVec.push_back(tempInt);
                    tempInt = "";
                }
            }
            sbox1.push_back(tempVec);
            tempVec.clear();
        }
        rfile_s1.close();}

    //initialize p4 vector (vec_p4)
        //open up the p_4 file
        std::ifstream rfile_p4;
        rfile_p4.open("./encryp/p_4.txt", std::ios::in);
        //get rid of the first 3 lines of comments in the text file
        std::getline(rfile_p4, temp); std::getline(rfile_p4, temp); std::getline(rfile_p4, temp);
        //get the permutation array in a vector
        while (rfile_p4 >> temp) {
            vec_p4.push_back(str_to_int(temp));
        }
        rfile_p4.close();

    //run initial permutation on the ciphertext
        //open up the IP file
        std::ifstream rfile_ip;
        rfile_ip.open("./encryp/IP.txt", std::ios::in);
        //get rid of first 3 lines of comments in the txt file
        std::getline(rfile_ip, temp); std::getline(rfile_ip, temp); std::getline(rfile_ip, temp);
        //get the permutation array in a vector
        while (rfile_ip >> temp) {
            px.push_back(str_to_int(temp));
        }
        ctxt = rearrange(px, ctxt);
        rfile_ip.close(); px.clear();

        std::cout << "Init. Permutation: " << ctxt << std::endl;

    //iterate based on number of rounds
    for (int x = 1; x < rounds; x++) {
        //run Expand and Permutate operation on the left side of the key
            lf_txt = rearrange(vec_ep, ctxt);

            std::cout << std::endl << "-- Function Fk --" << std::endl;
            std::cout << "Expd. Permutation: " << lf_txt << std::endl;

        //run XOR operation with lf_txt and key [key.size() - x-1] (vector k)
            lf_txt = x_or(lf_txt, k[k.size() - x]);
            //set middle of string to empty space
            lf_txt[(lf_txt.length()-1)/2] =  ' ';

            std::cout << "XOR with Key " << k.size() - x+1 <<  "   : " << lf_txt << std::endl;

        //run lf_txt through s-boxes
            //initialize temp to turn char into string (it wont work unless i do this)
            temp = "";
            //get sbox value from vector that represents the s-box matrix
            lf_txt = sbox0[BintoDec(temp+lf_txt[0]+lf_txt[3])][BintoDec(temp+lf_txt[1]+lf_txt[2])] + " " + sbox1[BintoDec(temp+lf_txt[5]+lf_txt[8])][BintoDec(temp+lf_txt[6]+lf_txt[7])];

            std::cout << "S-box            : " << lf_txt << std::endl;

        //run lf_txt through p4 permutation
            lf_txt = rearrange(vec_p4, lf_txt);

            std::cout << "P4               : " << lf_txt << std::endl;

        //run xor operation with lf_txt and left halve of the plaintext
            lf_txt.erase(2,1);
            lf_txt = x_or(lf_txt, ctxt);

            std::cout << "XOR with Left Key: " << lf_txt << std::endl <<std::endl;

        //switch operation
            ctxt.erase(0,(ctxt.length()+1)/2);
            ctxt = ctxt + " " + lf_txt;

            std::cout << "Switch Halves    : " << ctxt << std::endl;
    }

    ///final round
    //expand and permutate
        lf_txt = rearrange(vec_ep, ctxt);

        std::cout << std::endl << "-- Function Fk --" << std::endl;
        std::cout << "Expd. Permutation: " << lf_txt << std::endl;

    //xor with key 1 (k1)
        lf_txt = x_or(lf_txt, k.front());
        //set middle of string to empty space
        lf_txt[(lf_txt.length()-1)/2] =  ' ';

        std::cout << "XOR with Key 1   : " << lf_txt << std::endl;

    //sboxes
        //this part will need to be reworked to fit other sizes matrices
        //initialize temp to turn char into string (it wont work unless i do this)
        temp = "";
        //get sbox value from vector that represents the s-box matrix
        lf_txt = sbox0[BintoDec(temp+lf_txt[0]+lf_txt[3])][BintoDec(temp+lf_txt[1]+lf_txt[2])] + " " + sbox1[BintoDec(temp+lf_txt[5]+lf_txt[8])][BintoDec(temp+lf_txt[6]+lf_txt[7])];

        std::cout << "S-box            : " << lf_txt << std::endl;

    //p4 permutation
        lf_txt = rearrange(vec_p4, lf_txt);

        std::cout << "P4               : " << lf_txt << std::endl;

    //run xor operation with lf_txt and left halve of the plaintext
        lf_txt.erase(2,1);
        lf_txt = x_or(lf_txt, ctxt);

        std::cout << "XOR with Left Key: " << lf_txt << std::endl <<std::endl;

    //no need for switching, combine plaintext and run inverse IP (IP-1)
        //combine left and right halve
        ctxt.erase(0,(ctxt.length()+1)/2);
        ctxt = lf_txt + " " + ctxt;

        std::cout << "L+R combined     : " << ctxt << std::endl;
        //run inverse IP (IP-1)
        //open up the inverse IP file
        std::ifstream rfile_ip_1;
        rfile_ip_1.open("./encryp/IP-1.txt", std::ios::in);
        //get rid of first 3 lines of comments in the txt file
        std::getline(rfile_ip_1, temp); std::getline(rfile_ip_1, temp); std::getline(rfile_ip_1, temp);
        //get the permutation array in a vector
        while (rfile_ip_1 >> temp) {
            px.push_back(str_to_int(temp));
        }
        ctxt = rearrange(px, ctxt);
        rfile_ip_1.close(); px.clear();

        std::cout << "Inv. Permutation : " << ctxt << std::endl;

    return 0;
}
