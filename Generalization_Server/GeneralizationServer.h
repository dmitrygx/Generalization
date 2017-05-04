#pragma once

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <map>
#include <set>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <functional>

#include "GeneralizationRequestCurve.h"
#include "GeneralizationFile.h"
#include "GeneralizationDataBase.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::json;                                  // JSON library
using namespace std;

struct AlgorithmParams
{
	double C;
	uint32_t Np;
	uint32_t Ns;
	double f;
	uint32_t Ninit;
};

class GeneralizationServer
{
protected:
	http_listener listener;
	map<utility::string_t, utility::string_t> dictionary;

	typedef enum Objects
	{
		UNDEFINED		= -1,
		INITIALIZE		= 0,
		SOURCE_CURVE		= 1,
		ADDUCTION_CURVE		= 2,
		SEGMENTATION_CURVE	= 3,
		SIMPLIFICATION_CURVE	= 4,
		SMOOTHING_CURVE		= 5
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

	int InitializeCurves(string_t storage_type, string_t storage_path,
		AlgorithmParams *algParams);

	GeneralizationFile GenFile;
	GeneralizationDataBase GenDataBase;
	curves CurvesMap;

	GeneralizationRequestCurve *Curves;

	boolean running;
public:
	GeneralizationServer(const http::uri& url);
	DWORD start();
	
	virtual ~GeneralizationServer();

	void ChageStateOfServerThread(boolean state)
	{
		running = state;
	}

	GeneralizationFile* GetGeneralizationFile()
	{
		return &GenFile;
	}

	GeneralizationDataBase* GetGeneralizationDataBase()
	{
		return &GenDataBase;
	}
	void AllocateMemCurves(size_t Count, double_t C_, uint32_t Np_,
		uint32_t Ns_, double_t f_, uint32_t Ninit_);
};

