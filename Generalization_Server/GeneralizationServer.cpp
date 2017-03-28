#include "GeneralizationServer.h"
//http_listener(L"http://localhost/generalization_server"))
GeneralizationServer::GeneralizationServer(const http::uri& url) : listener(http_listener(url))
{
	listener.support(methods::GET, std::bind(&GeneralizationServer::handle_get,
						 this,
						 std::tr1::placeholders::_1));
	listener.support(methods::POST, std::bind(&GeneralizationServer::handle_post,
						 this,
						 std::tr1::placeholders::_1));
	listener.support(methods::PUT, std::bind(&GeneralizationServer::handle_put,
						 this,
						 std::tr1::placeholders::_1));
	listener.support(methods::DEL, std::bind(&GeneralizationServer::handle_del,
						 this,
						 std::tr1::placeholders::_1));
	/*listener.support([](http_request request)
	{
		request.reply(status_codes::OK, U("Hello, World!"));

	});*/
	initialized = false;

	allowedPath[static_cast<string_t>(U("initialize"))] = INITIALIZE;
	allowedPath[static_cast<string_t>(U("source_curve"))] = SOURCE_CURVE;


	try
	{
		listener
			.open()
			.then([&]() {cout << "\nstarting to listen\n" << endl; })
			.wait();

		while (true)
		{
			
		}
	}
	catch (exception const & e)
	{
		cout << e.what() << endl;
	}

}

GeneralizationServer::Objects_t 
GeneralizationServer::getObjectFromString(string_t &string, char_t nestingLevel)
{
	(void) nestingLevel;

	if (allowedPath.find(string) == allowedPath.end())
	{
		return GeneralizationServer::Objects_t::UNDEFINED;
	}

	return allowedPath[string];
}

size_t GeneralizationServer::checkPath(vector<string_t> &splittedPath)
{
	if (0 != strcmp(utility::conversions::to_utf8string(splittedPath[0]).c_str(), 
		"generalization_server"))
	{
		wcout << "Received unknown request: " << splittedPath[0] << endl;
		return -1;
	}

	return 0;
}

void GeneralizationServer::InitializeCurves()
{
	GenFile.ParseAllDataInFile();
	CurvesMap = GenFile.GetCurves();

	Curves = new GeneralizationRequestCurve[GenFile.GetNumberOfCurves()];

	for (uint32_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
	{
		Curves[i].SetCurve(X(CurvesMap[i]).size(), &CurvesMap[i]);
	}
}

void GeneralizationServer::handle_request(http_request request,
	function<void(json::value &, json::value &)> action)
{
	json::value answer;

	wcout << request.relative_uri().to_string() << endl;

	auto http_get_vars = uri::split_query(request.request_uri().query());
	auto http_get_path = uri::split_path(request.request_uri().path());
	wcout << "PATH : " << endl;
	wcout << request.request_uri().path() << endl;
	wcout << "QUERY : " << endl;
	wcout << request.request_uri().query() << endl;
	/*auto found_name = http_get_vars.find(U("foo"));*/

	//if (found_name == end(http_get_vars)) {
	//	auto err = U("Request received with get var \"request\" omitted from query.");
	//	wcout << err << endl;
	//	/* BAD */
	//	request.reply(status_codes::OK, answer);
	//	return;
	//}

	/*auto request_name = found_name->second;*/
	/*wcout << U("Received request: ") << request_name << endl;*/


	if (checkPath(http_get_path) < 0)
		goto err;

	char_t nestingLevel = 0;
	Objects_t reqObj;
	for (string_t reqObjStr : http_get_path)
	{
		reqObj = getObjectFromString(reqObjStr, nestingLevel);
		wcout << reqObjStr << endl;
		nestingLevel++;
	}
	/* TODO: move to @handle_get func */
	if ((reqObj == INITIALIZE) && (!initialized))
	{
		InitializeCurves();

		auto array = answer.array();
		json::value root;
		for (size_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
		{
			web::json::value obj;

			obj[L"count_points"] = web::json::value::number(X(CurvesMap[i]).size());

			array[i] = obj;
		}
		root[L"curves"] = array;
		request.reply(status_codes::OK, root);

		initialized = true;
		return;
	}
	else if ((reqObj == SOURCE_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));

		if (found_curve == end(http_get_vars)) {
			auto err = U("Request received with get var \"request\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		wcout << U("Received request: ") << request_curve << endl;
		uint32_t requested_curve_number = atoi((char*)request_curve.c_str());
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];
		uint32_t size = X(CurvesMap[requested_curve_number]).size();
		curve *reqCurve = &CurvesMap[requested_curve_number];
		requested_curve->SetCurve(size, reqCurve);
		requested_curve->DispatchEvent(Event_t::INITIALIZE);
		
		auto array = answer.array();
		json::value root;
		curve *source_curve = requested_curve->GetSouceCurve();
		root[L"count_points"] = X(*source_curve).size();
		for (size_t i = 0; i < X(*source_curve).size(); i++)
		{
			web::json::value x;
			web::json::value y;

			x = web::json::value::number(X(*source_curve)[i]);
			y = web::json::value::number(Y(*source_curve)[i]);

			web::json::value pointXY;
			pointXY[L"X"] = x;
			pointXY[L"Y"] = y;

			array[i] = pointXY;
		}
		root[L"points"] = array;
		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}

	/*std::cout << utility::conversions::to_utf8string(request.to_string()) << endl;*/

	//request
	//	.extract_json()
	//	.then([&answer, &action](pplx::task<json::value> task) {
	//		try
	//		{
	//			auto & jvalue = task.get();

	//			if (!jvalue.is_null())
	//			{
	//				action(jvalue, answer);
	//			}
	//		}
	//		catch (http_exception const & e)
	//		{
	//			wcout << e.what() << endl;
	//		}
	//	})
	//	.wait();
	/*
	request.reply(status_codes::OK, answer);*/
err:
	request.reply(status_codes::BadRequest);
}

void GeneralizationServer::handle_get(http_request request)
{
	std::cout << "\nhandle GET\n" << std::endl;
	
	handle_request(
		request,
		[&](json::value & jvalue, json::value & answer)
		{
			auto http_get_vars = uri::split_query(request.request_uri().query());
			wcout << "QUERY : " << endl;
			wcout << request.request_uri().query() << endl;
			auto found_name = http_get_vars.find(U("/curve"));

			if (found_name == end(http_get_vars)) {
				auto err = U("Request received with get var \"request\" omitted from query.");
				wcout << err << endl;
				/* BAD */
				return;
			}

			auto request_name = found_name->second;
			wcout << U("Received request: ") << request_name << endl;
		}
	);

	/*for (auto const & p : dictionary)
	{
		answer[p.first] = json::value::string(p.second);
		std::cout << utility::conversions::to_utf8string(p.second) << std::endl;
	}*/

	/*request.reply(status_codes::OK, answer);*/
}

void GeneralizationServer::handle_post(http_request request)
{
	std::cout << "\nhandle POST\n" << std::endl;

	handle_request(
		request,
		[](json::value & jvalue, json::value & answer)
		{
			//for (auto const & e : jvalue)
			//{
			//	if (e.second.is_string())
			//	{
			//		auto key = e.second.as_string();

			//		/*if (pos == dictionary.end())
			//		{
			//			answer.push_back(make_pair(json::value(key), json::value(L"<nil>")));
			//		}
			//		else
			//		{
			//			answer.push_back(make_pair(json::value(pos->first), json::value(pos->second)));
			//		}*/
			//	}
			//}
			wcout << jvalue.serialize() << endl;
		});
}

void GeneralizationServer::handle_put(http_request request)
{
	std::cout << "\nhandle PUT\n" << std::endl;

	/*handle_request(
		request,
		[](json::value & jvalue, json::value::field_map & answer)
	{
		for (auto const & e : jvalue)
		{
			if (e.first.is_string() && e.second.is_string())
			{
				auto key = e.first.as_string();
				auto value = e.second.as_string();

				if (dictionary.find(key) == dictionary.end())
				{
					TRACE_ACTION(L"added", key, value);
					answer.push_back(make_pair(json::value(key), json::value(L"<put>")));
				}
				else
				{
					TRACE_ACTION(L"updated", key, value);
					answer.push_back(make_pair(json::value(key), json::value(L"<updated>")));
				}

				dictionary[key] = value;
			}
		}
	}
	);*/
}

void GeneralizationServer::handle_del(http_request request)
{
	std::cout << "\nhandle DEL\n" << std::endl;

	/*handle_request(
		request,
		[](json::value & jvalue, json::value::field_map & answer)
	{
		set<utility::string_t> keys;
		for (auto const & e : jvalue)
		{
			if (e.second.is_string())
			{
				auto key = e.second.as_string();

				auto pos = dictionary.find(key);
				if (pos == dictionary.end())
				{
					answer.push_back(make_pair(json::value(key), json::value(L"<failed>")));
				}
				else
				{
					TRACE_ACTION(L"deleted", pos->first, pos->second);
					answer.push_back(make_pair(json::value(key), json::value(L"<deleted>")));
					keys.insert(key);
				}
			}
		}

		for (auto const & key : keys)
			dictionary.erase(key);
	}
	);*/
}


GeneralizationServer::~GeneralizationServer()
{
}
