#pragma once

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <map>
#include <set>
#include <vector>

#include "GeneralizationRequestCurve.h"
#include "GeneralizationFile.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::json;                                  // JSON library
using namespace std;

class GeneralizationServer
{
protected:
	http_listener listener;
	map<utility::string_t, utility::string_t> dictionary;

	typedef enum Objects
	{
		UNDEFINED = -1,
		INITIALIZE = 0,
		SOURCE_CURVE = 1,
		ADDUCTION_CURVE = 2,
	} Objects_t;

	map <string_t, Objects_t> allowedPath;
	bool initialized;

	void handle_get(http_request request);
	void handle_put(http_request request);
	void handle_post(http_request request);
	void handle_del(http_request request);
	void handle_request(http_request request,
		function<void(json::value &, json::value &)> action);

	Objects_t getObjectFromString(string_t &string, char_t nestingLevel);
	size_t checkPath(vector<string_t> &splittedPath);

	void InitializeCurves();

	GenaraliztionFile GenFile;
	curves CurvesMap;

	GeneralizationRequestCurve *Curves;
public:
	GeneralizationServer(const http::uri& url);
	virtual ~GeneralizationServer();
};

