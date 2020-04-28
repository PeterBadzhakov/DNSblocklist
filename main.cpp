#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h>

using std::string;

#pragma comment(lib, "ws2_32.lib")

void init_net();
void clean_net();

string flip_ip(const char* ip);

void print(const char* ip_text);
void print(const char* const* address_list);
void print(const hostent* query);

int main(int argc, const char** argv)
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " {<IPv4>}+\n";
        return 0;
    }
    ++argv;

    init_net();

    for (; *argv != nullptr; ++argv)
    {
        string domain_name = flip_ip(*argv);
        domain_name += ".zen.spamhaus.org";
        
        const hostent* query = gethostbyname(domain_name.c_str());
        
        std::cout
            << "The IP address: "
            << argv[0];
        if (query == nullptr || query->h_addr_list[0] == nullptr)
        {
                std::cout << " is NOT found in the Spamhaus blacklists.\n\n";
        }
        else
        {
            std::cout << " is found in the following Spamhaus public IP zone:\n";
            print(query->h_addr_list);

            std::cout << '\n';
        }
    }

    clean_net();
	return 0;
}

// Link up WinSock dynamics.
void init_net()
{
    WSADATA sock_pod;
    const int res = WSAStartup(MAKEWORD(2, 2), &sock_pod);

    if (res != 0)
    {
        std::cerr << "WSAStartup failed: " << res << '\n';
        exit(res);
    }
}

// Terminate use of WinSock dll.
void clean_net()
{
    WSACleanup();
}

// Assume ip is in text-form IPv4, else UB.
string flip_ip(const char* ip)
{
    string res;
    // Get iterator to end of ip string.
    const char* end;
    for (end = &ip[0]; end[1] != '\0'; ++end)
    {
        ;
    }

    // Find octet from right to left...
    const char* copy_from = nullptr;
    for (const char* rev_it = end; true; --rev_it)
    {
        // If reached leftmost octet, copy it and exit.
        if (rev_it == &ip[0])
        {
            for (copy_from = rev_it; *copy_from != '.'; ++copy_from)
            {
                res.push_back(*copy_from);
            }
            return res;
        }
        // Else copy the current one and pad with '.'.
        else if (*rev_it == '.')
        {
            for (copy_from = rev_it + 1; copy_from <= end && *copy_from != '.'; ++copy_from)
            {
                res.push_back(*copy_from);
            }
            res.push_back('.');
        }
    }
}

void print(const char* ip_text)
{
    std::cout
        << int(ip_text[0]) << '.'
        << int(ip_text[1]) << '.'
        << int(ip_text[2]) << '.'
        << int(ip_text[3]);
}

void print(const char* const* address_list)
{
    /*
    127.0.0.2	SBL	Spamhaus SBL Data
    127.0.0.3	SBL	Spamhaus SBL CSS Data
    127.0.0.4-7	XBL	CBL Data
    127.0.0.9	SBL	Spamhaus DROP/EDROP Data
    127.0.0.10	PBL	ISP Maintained
    127.0.0.11	PBL	Spamhaus Maintained
    */

    for (; *address_list != nullptr; ++address_list)
    {
        print(address_list[0]);
        
        switch (int(address_list[0][3]))
        {
        case 2:
            std::cout << " SBL	Spamhaus SBL Data.";
            break;
        case 3:
            std::cout << " SBL	Spamhaus SBL CSS Data.";
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            std::cout << " XBL	CBL Data.";
            break;
        case 9:
            std::cout << " SBL	Spamhaus DROP/EDROP Data.";
            break;
        case 10:
            std::cout << " PBL	ISP Maintained.";
            break;
        case 11:
            std::cout << " PBL	Spamhaus Maintained.";
            break;
        default:
            std::cout << " Unknown.";
            std::cerr << " DNS query error!";
            break;
        }

        std::cout << '\n';
    }
}

void print(const hostent* query)
{
    std::cout <<
        query->h_name << '\n' <<
        query->h_length << '\n';
    
    for (auto it = query->h_aliases; *it != nullptr; ++it)
    {
        std::cout << *it << '\n';
    }
    
    for (const char* const* it = query->h_addr_list; *it != nullptr; ++it)
    {
        print(it[0]);
    }
    
    std::cout << '\n';
}
