#include "ip_calc.h"

unsigned char GetNetmaskLength(const int * ip)
{
    unsigned char c = 0;

    for(unsigned char k = 0; k<4; ++k)
    {
    if(ip[k]<255)
    {
        for(int i = 0; i<8; ++i)
        {
            if((ip[0] & 1>>i)!=0)
            {c++;}
        }   
        return c;
    }
    else{c += 8;}
    }
    return c;
}

int* SplitIntoDigits(const string& text)
{
    int* digits = new int[4];
    string word = "";
    int counter = 0;
    int i = 0;
    while(i < text.length())
    {
        if(text[i] == '.' || text[i] == ',' || text[i] == ' ' || text[i] == '\0')
        {
                digits[counter]=stoi(word);
                word = "";
                ++counter;
        }
        else
        {word += text[i];}
        ++i;
    }
    digits[counter]=stoi(word);
    return digits;
}

bool IsValidIpAddress(const int * ip)
{
    bool ret = true;
    for(int i = 0; i<4; ++i)
    {
        if(ip[i]>255 || ip[i]<0)
        ret = false;
    }
    return ret;
}

bool IsValidNetmask(const int * netmask)
{
    bool valid_nm = true;
    for(int i = 0; i<4; ++i)
    {
        if
        (
            netmask[i]!=0   &&
            netmask[i]!=128 &&
            netmask[i]!=192 &&
            netmask[i]!=224 &&
            netmask[i]!=240 &&
            netmask[i]!=248 &&
            netmask[i]!=252 &&
            netmask[i]!=254 &&
            netmask[i]!=255
        )
        {valid_nm = false;}

        if
        (
            (netmask[0]<255 && netmask[1]!=0) ||
            (netmask[1]<255 && netmask[2]!=0) ||
            (netmask[2]<255 && netmask[3]!=0)
        )
        {valid_nm = false;}
    }
    return valid_nm;
}

void SetIpAddress(ipv4& a)
{
    string ip;
    bool valid_ip;
    int* mas;

    do
    {
        valid_ip = true;
        ip = "";
        std::cout << "Enter the IPv4 address: ";
        getline(cin, ip);
        mas = SplitIntoDigits(ip);

        if(IsValidIpAddress(mas))
        {
            for(int i = 0; i<4; ++i)
            {a.ip[i]=mas[i];}  //конечное изменение элемента структуры
            delete[] mas;
        }
        else
        {
            valid_ip = false;
            cout << "Wrong IP Address!" << endl;
        }
    }
    while(valid_ip!=true);
}

void SetNetmask(ipv4& a)
{
    string nm;
    bool valid_nm;
    int* mas;

    do
    {
        valid_nm = true;
        nm = "";
        std::cout << "Enter the netmask: ";
        getline(cin, nm);

        if(nm.length()>3 && nm[0]!='/')
        {
            mas = SplitIntoDigits(nm);
            if(IsValidNetmask(mas))
            {
                for(int i = 0; i<4; ++i)
                {a.netmask[i]=mas[i];}  //конечное изменение элемента структуры
                a.netmask_length = GetNetmaskLength(mas);
                delete[] mas;
            }
            else
            {
                valid_nm = false;
                cout << "Wrong netmask!" << endl;
            }
        }

        if(nm.size()<4 && nm[0]=='/')
        {
            string temp = "";
            for(int i = 1; i<4; ++i)
            {temp+=nm[i];}
            a.netmask_length = stoi(temp);

            if((int)a.netmask_length > 32 || (int)a.netmask_length < 0)
            {
                valid_nm = false;
                cout << "Wrong netmask!" << endl;
            }
            else
            {
                if(a.netmask_length<33 && a.netmask_length>24)
                {
                    a.netmask[0]=255;
                    a.netmask[1]=255;
                    a.netmask[2]=255;
                    a.netmask[3]=256-(1 << (32-a.netmask_length));
                }

                if(a.netmask_length<25 && a.netmask_length>16)
                {
                    a.netmask[0]=255;
                    a.netmask[1]=255;
                    a.netmask[2]=256-(1 << (24-a.netmask_length));
                    a.netmask[3]=0;
                }

                if(a.netmask_length<17 && a.netmask_length>8)
                {
                    a.netmask[0]=255;
                    a.netmask[1]=256-(1 << (16-a.netmask_length));
                    a.netmask[2]=0;
                    a.netmask[3]=0;
                }

                if (a.netmask_length < 9 && a.netmask_length > -1)
                {
                    a.netmask[0]=256-(1 << (8-a.netmask_length));
                    a.netmask[1]=0;
                    a.netmask[2]=0;
                    a.netmask[3]=0;
                }
            }
        }

        if(nm.length()<4 && nm[0]!='/')
        {
            valid_nm = false;
            cout << "Wrong netmask!" << endl;
        }
    }
    while(valid_nm!=true);
}

void ShowAddress(unsigned char * mas)
{
    string answer;

    for(int i = 0; i<4; ++i)
    {answer+=to_string((int)mas[i])+".";}
    answer.pop_back();

    while(answer.size()!=20)
    {answer += " ";}

    for(int i = 0; i<4; ++i)
    {
        for(int j = 7; j >-1; --j)
        {
            answer+=to_string((bool)(mas[i] & 1<<j));
        }
        answer+=".";
    }
    answer.pop_back();
    answer+="\t";
    cout << answer;
}

void SetWildcard(ipv4& a)
{
    for(int i=0;i<4;++i)
    {a.wildcard[i] = ~a.netmask[i];}
}

void SetNetwork(ipv4& a)
{
    for(int i=0;i<4;++i)
    {a.network[i] = a.ip[i] & a.netmask[i];}
}

void SetBroadcast(ipv4& a)
{
    for(int i=0;i<4;++i)
    {a.broadcast[i] = a.network[i] | a.wildcard[i];}
}

void SetFirstAddress(ipv4& a)
{
    if(a.network[3]<255)
    {
        for(int i=0;i<4;++i)
        {a.first_address[i] = a.network[i];}
        a.first_address[3]++;
    }
    else
    {
        for(int i=0;i<4;++i)
        {a.first_address[i] = a.network[i];}
    }
}

void SetLastAddress(ipv4& a)
{
    for(int i=0;i<4;++i)
    {a.last_address[i] = a.broadcast[i];}
    a.last_address[3]--;
}

void ComputeIpv4(ipv4& a)
{
    SetIpAddress(a);    
    SetNetmask(a);                  
    SetWildcard(a);     
    SetNetwork(a);      
    SetBroadcast(a);    
    SetFirstAddress(a);
    SetLastAddress(a);  

    ShowAddress(a.ip);              cout << "IP" << endl;
    ShowAddress(a.netmask);         cout << "NETMASK" << endl;
    ShowAddress(a.wildcard);        cout << "WILDCARD" << endl;
    ShowAddress(a.network);         cout << "NETWORK" << endl;
    ShowAddress(a.broadcast);       cout << "BROADCAST" << endl;
    ShowAddress(a.first_address);   cout << "FIRST ADDRESS" << endl;
    ShowAddress(a.last_address);    cout << "LAST ADDRESS" << endl;

    cout << endl;

    int count = (int)(a.netmask_length);
    if(count == 32)
    {
        cout << "ADRESSES COUNT = " << 0 <<'\n';
        cout << "HOSTS COUNT    = " << 0 <<'\n';
    }
    else
    {   
        cout << "ADRESSES COUNT = " <<  (1ULL<<(32-count)) <<'\n';
        cout << "HOSTS COUNT    = " <<  (1ULL<<(32-count))-2 <<'\n';
    }
    
}


                        