#include "soapH.h"
#include "LeafNetsWebServiceSoapBinding.nsmap"
#include <stdbool.h>

void getNetworks();

main()
{
	/*
   struct soap *soap = soap_new();
   soap->userid = "u1";
   soap->passwd = "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8";
   bool result;
   struct ns1__getContactResponse contactResponse;
   struct ns2__Contact contact;
   // u1 86f7e437faa5a7fce15d1ddcb9eaeaea377667b8
   //if (soap_call_ns1__vrfyLogin(soap, NULL, NULL, &result) == SOAP_OK)
   if (soap_call_ns1__getContact(soap, NULL, NULL, "u1", &contactResponse) == SOAP_OK)
      printf("contact %s\n ", contactResponse._getContactReturn->email);
   else
      soap_print_fault(soap, stderr);
   soap_end(soap);
   soap_free(soap);
	*/
	getNetworks();
} 

void getNetworks() {
   struct soap *soap = soap_new();
   soap->userid = "u1";
   soap->passwd = "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8";
   struct ns1__getNetworksResponse networksResponse;
   if (soap_call_ns1__getNetworks(soap, NULL, NULL, "u1", &networksResponse) == SOAP_OK) {
	struct ArrayOf_USCORExsd_USCOREanyType* networks = networksResponse._getNetworksReturn;
	int i = 0;
	for ( i = 0; i < networks->__size; ++i ) {
		//struct ns2__Network* nsNetwork = (struct ns2__Network *)networks->__ptr[i];
		char* a = networks->__ptr[i];
		printf( "Network %d\n", strlen(a) );
	}	
   } else {
      soap_print_fault(soap, stderr);
   }
   soap_end(soap);
   soap_free(soap);
} 

