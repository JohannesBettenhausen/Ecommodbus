/*
 * Rest.h
 *
 *  Created on: 20.09.2018
 *      Author: johannes
 */

#ifndef REST_H_
#define REST_H_
#include <cpprest/http_client.h>

namespace mod {

class Rest {

private:

	//web::http::client::http_client *client(("http//localhost"));

public:
	Rest();
	pplx::task<void> hochladen();
	void lala();
};

} /* namespace mod */

#endif /* REST_H_ */
