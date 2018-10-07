/*
 * Rest.cpp
 *
 *  Created on: 20.09.2018
 *      Author: johannes
 */
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "Rest.h"
using namespace web;
using namespace web::http;
using namespace web::http::client;
using concurrency::streams::file_stream;
using concurrency::streams::basic_istream;

#include <iostream>
using namespace std;

namespace mod {

Rest::Rest(string daten[]) {
	// TODO Auto-generated constructor stub
	datenhochladen(daten);

}

void Rest::datenhochladen(string daten[]){

	json::value postData;

	for(int i=0;i<15;i++){

		postData[this->datenart[i]]=json::value::string(U(daten[i]));

	}
	jsonhochladen(postData);
}

void Rest::jsonhochladen(json::value postData){

	http_client client(("http://127.0.0.1:8000"));
	http_response response = client.request(methods::POST,"messdaten/",postData.to_string().c_str(),"application/json").get();

	if(response.status_code() == status_codes::OK)
	{
	  auto body = response.extract_string();
	  std::wcout << L"Added new Id: " << body.get().c_str() << std::endl;

	}

}

} /* namespace mod */
