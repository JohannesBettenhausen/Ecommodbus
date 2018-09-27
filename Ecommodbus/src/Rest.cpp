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
#include "Rest.h"
using namespace web;
using namespace web::http;
using namespace web::http::client;
using concurrency::streams::file_stream;
using concurrency::streams::basic_istream;

#include <iostream>
using namespace std;

namespace mod {

Rest::Rest() {
	// TODO Auto-generated constructor stub

}

void Rest::lala(){

	printf("llallala");
}

pplx::task<void> Rest::hochladen()
{

    using concurrency::streams::file_stream;
    using concurrency::streams::basic_istream;


    return file_stream<unsigned char>::open_istream("messdaten.json").then([](pplx::task<basic_istream<unsigned char>> previousTask)
    {
        try
        {
            auto fileStream = previousTask.get();

            http_client client("http://127.0.0.1:8000");
            return client.request(methods::PUT, "messdaten.json", fileStream).then([fileStream](pplx::task<http_response> previousTask)
            {
                fileStream.close();

                std::wostringstream ss;
                try
                {
                    auto response = previousTask.get();
                    ss << L"Server returned returned status code " << response.status_code() << L"." << std::endl;
                }
                catch (const http_exception& e)
                {
                    ss << e.what() << std::endl;
                }
                std::wcout << ss.str();
            });
        }
        catch (const std::system_error& e)
        {
            std::wostringstream ss;
            ss << e.what() << std::endl;
            std::wcout << ss.str();


            return pplx::task_from_result();
        }
    });

}


} /* namespace mod */
