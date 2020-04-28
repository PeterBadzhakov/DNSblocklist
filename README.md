# DNSblocklist
Small program to check given IPs in blocklists; spamhaus DNS.

# Building
Open the solution with visual studio or compile main.cpp directly. 

Tested working on Microsoft Visual Studio Enterprise 2019 on Windows 10, 
x64 build on ISO C++17 standard.

All required libraries come with a standard installation of Visual Studio for
desktop Windows development.

# Example usage:
$ CN.exe 127.0.0.2 36.41.122.183 1.1.1.1

The IP address: 127.0.0.2 is found in the following Spamhaus public IP zone:

127.0.0.4 XBL   CBL Data.

127.0.0.2 SBL   Spamhaus SBL Data.

127.0.0.10 PBL  ISP Maintained.


The IP address: 36.41.122.183 is found in the following Spamhaus public IP zone:

127.0.0.11 PBL  Spamhaus Maintained.


The IP address: 1.1.1.1 is NOT found in the Spamhaus blacklists.
