#include "GeneralizationServer.h"
#include "Wininet.h"
#ifdef _OPENMP
#include "omp.h"
#endif

GeneralizationServer::GeneralizationServer(string Path, string Type,
	double C, uint32_t Np, uint32_t Ns, double f, uint32_t Ninit)
{
	parallel_algortihm = false;

	AlgorithmParams algParams;
	algParams.C = C;
	algParams.Np = Np;
	algParams.Ns = Ns;
	algParams.f = f;
	algParams.Ninit = Ninit;

	InitializeCurves(utility::conversions::to_utf16string(Type),
		utility::conversions::to_utf16string(Path), &algParams);
}

//http_listener(L"http://localhost/generalization_server"))
GeneralizationServer::GeneralizationServer(const http::uri& url) : listener(http_listener(url))
{
	parallel_algortihm = false;

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
	allowedPath[static_cast<string_t>(U("adduction_curve"))] = ADDUCTION_CURVE;
	allowedPath[static_cast<string_t>(U("segmentation_curve"))] = SEGMENTATION_CURVE;
	allowedPath[static_cast<string_t>(U("simplification_curve"))] = SIMPLIFICATION_CURVE;
	allowedPath[static_cast<string_t>(U("smoothing_curve"))] = SMOOTHING_CURVE;
	allowedPath[static_cast<string_t>(U("save_curve"))] = SAVE_CURVE;
	allowedPath[static_cast<string_t>(U("generalization_curve"))] = GENERALIZE_CURVE;
	allowedPath[static_cast<string_t>(U("benchmark"))] = BENCHMARK;

	running = true;
	/*try
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
	}*/
}

DWORD GeneralizationServer::start()
{
	try
	{
		listener
			.open()
			.then([&]() {cout << "\nstarting to listen\n" << endl; })
			.wait();

		while (running)
		{
		}
	}
	catch (exception const & e)
	{
		cout << e.what() << endl;
	}

	return 0;
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

string* UpdateFilePath(string fileName)
{
	typedef vector< string > split_vector_type;

	split_vector_type SplitVec1;
	string output;
	boost::replace_all(fileName, "%3A", ":");
	boost::replace_all(fileName, "%5C", "\\");

	string *finalfileName = new string;
	*finalfileName = fileName;
	///*boost::split(SplitVec1, fileName, boost::is_any_of("%3A"), boost::token_compress_on);*/
	//string preFinalfileName;
	//for (string part : SplitVec1)
	//{
	//	preFinalfileName += part;
	//}

	//split_vector_type SplitVec2;
	//boost::split(SplitVec2, preFinalfileName, boost::is_any_of("%5C"), boost::token_compress_on);
	//string *finalfileName = new string;
	//for (string part : SplitVec2)
	//{
	//	*finalfileName += part;
	//}

	return finalfileName;
}

void GeneralizationServer::AllocateMemCurves(size_t Count, double_t C_, uint32_t Np_,
	uint32_t Ns_, double_t f_, uint32_t Ninit_)
{
	void* raw_memory = operator new[](Count * sizeof(GeneralizationRequestCurve));
	Curves = static_cast<GeneralizationRequestCurve*>(raw_memory);
	for (size_t i = 0; i < Count; ++i) {
		new(&Curves[i])GeneralizationRequestCurve(C_, Np_, Ns_, f_, Ninit_, parallel_algortihm);
	}
}

void GeneralizationServer::InitializeBenchmark(uint32_t pps, uint32_t min_seg_cnt,
	uint32_t max_seg_cnt, AlgorithmParams &algParams)
{
	uint32_t Count = max_seg_cnt - min_seg_cnt + 1;
	void* raw_memory = operator new[](Count * sizeof(GeneralizationBenchmark));
	BenchCurves = static_cast<GeneralizationBenchmark*>(raw_memory);
	uint32_t iter_thr_mem = 0;
	for (size_t i = min_seg_cnt; i <= max_seg_cnt; ++i) 
	{
		new(&BenchCurves[iter_thr_mem])GeneralizationBenchmark(pps, i, algParams.C,
			algParams.Np, algParams.Ns, algParams.f,
			algParams.Ninit, algParams.OpenMP);
		iter_thr_mem++;
	}
}

void GeneralizationServer::FinalizeBenchmark()
{
	void* raw_memory = static_cast<void*>(BenchCurves);
	free(raw_memory);
}

#define MethodInvoke(type, method)	\
	(Gen##type .method)

void GeneralizationServer::HandleAllCurves(string type)
{
	size_t curve_number = (type == "File") ?
		MethodInvoke(File, GetNumberOfCurves()) :
		MethodInvoke(DataBase, GetNumberOfCurves());

#pragma omp parallel for if ((parallel_algortihm) && (curve_number >= 4))
	for (int64_t i = 0; i < curve_number; i++)
	{
		GeneralizationRequestCurve *requested_curve = &Curves[i];
		uint32_t size = X(CurvesMap[i]).size();
		curve *reqCurve = &CurvesMap[i];
		requested_curve->SetCurve(size, reqCurve);
		if (size != 0)
		{
			requested_curve->DispatchEvent(Event_t::INITIALIZE);
			requested_curve->DispatchEvent(Event_t::ADDUCTION);
			requested_curve->DispatchEvent(Event_t::SEGMENTATION);
			requested_curve->DispatchEvent(Event_t::SIMPLIFICATION);
			requested_curve->DispatchEvent(Event_t::SMOOTHING);
		}
	}

	for (int64_t i = 0; i < curve_number; i++)
	{
		GeneralizationRequestCurve *requested_curve = &Curves[i];
		uint32_t size = X(CurvesMap[i]).size();
		if ((update_db) && (type == "DataBase") && (size != 0))
		{
			GenDataBase.UpdateDataBaseObject_UpdateMetric(requested_curve);
		}

		if (size != 0)
			requested_curve->OutputResults();
	}
}

int GeneralizationServer::HandleCurve(string type, string Code, long Number)
{
	size_t curve_number = (type == "File") ?
		MethodInvoke(File, GetNumberOfCurves()) :
		MethodInvoke(DataBase, GetNumberOfCurves());
	int found = -1;
	/* if ((parallel_algortihm) && (curve_number >= 4))*/
//#pragma omp parallel for
	for (int64_t i = 0; (i < curve_number) && (found == -1); i++)
	{
		GeneralizationRequestCurve *requested_curve = &Curves[i];
		if ((requested_curve->GetDBCode() == Code) &&
			(requested_curve->GetDBNumber() == Number))
		{
			uint32_t size = X(CurvesMap[i]).size();
			curve *reqCurve = &CurvesMap[i];
			requested_curve->SetCurve(size, reqCurve);
			if (size != 0)
			{
				requested_curve->DispatchEvent(Event_t::INITIALIZE);
				requested_curve->DispatchEvent(Event_t::ADDUCTION);
				requested_curve->DispatchEvent(Event_t::SEGMENTATION);
				requested_curve->DispatchEvent(Event_t::SIMPLIFICATION);
				requested_curve->DispatchEvent(Event_t::SMOOTHING);
				if ((update_db) && (type == "DataBase"))
				{
					GenDataBase.UpdateDataBaseObject(requested_curve);
				}
				requested_curve->OutputResults();
			}
			found = 0;
		}
	}
	return found;
}

int GeneralizationServer::InitializeCurves(string_t storage_type, string_t storage_path,
	AlgorithmParams *algParams)
{
	int res = -1;

	if (storage_type == U("File"))
	{
		std::string fileName = utility::conversions::to_utf8string(storage_path);
		std::string *resultFileName = UpdateFilePath(fileName);

		GenFile.SetPath(*resultFileName);

		res = GenFile.ParseAllDataInFile();
		if (res != 0)
			return res;
		CurvesMap = GenFile.GetCurves();
		AllocateMemCurves(GenFile.GetNumberOfCurves(), algParams->C,
			algParams->Ns, algParams->Np, algParams->f, algParams->Ninit);

		for (uint32_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
		{
			Curves[i].SetCurve(X(CurvesMap[i]).size(), &CurvesMap[i]);
			Curves[i].SetDBInfo("0", i);
			Curves[i].SetUseOpenMP(algParams->OpenMP);
			Curves[i].SetUseMkl(algParams->IntelMKL);
		}

		res = 0;
	}
	else if (storage_type == U("DataBase"))
	{
		std::string fileName = utility::conversions::to_utf8string(storage_path);
		std::string *resultFileName = UpdateFilePath(fileName);

		GenDataBase.setFileName(*resultFileName);

		cout << "Result filename is " << *resultFileName << endl;

		delete resultFileName;

		res = GenDataBase.ParseAllDataInDB();
		if (res != 0)
			return res;
		CurvesMap = GenDataBase.GetCurves();

		AllocateMemCurves(GenDataBase.GetNumberOfCurves(), algParams->C,
			algParams->Ns, algParams->Np, algParams->f, algParams->Ninit);


		for (uint32_t i = 0; i < GenDataBase.GetNumberOfCurves(); i++)
		{
			Curves[i].SetCurve(X(CurvesMap[i]).size(), &CurvesMap[i]);
			Curves[i].SetDBInfo(GenDataBase.GetDBCode(i), GenDataBase.GetDBNumber(i));
			Curves[i].SetUseOpenMP(algParams->OpenMP);
			Curves[i].SetUseMkl(algParams->IntelMKL);
		}

		res = 0;
	}

	return res;
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
		auto found_storage_type = http_get_vars.find(U("type"));
		auto found_storage_path = http_get_vars.find(U("path"));
		auto found_alg_params_OpenMP = http_get_vars.find(U("params_OpenMP"));
		auto found_alg_params_C = http_get_vars.find(U("params_C"));
		auto found_alg_params_Np = http_get_vars.find(U("params_Np"));
		auto found_alg_params_Ns = http_get_vars.find(U("params_Ns"));
		auto found_alg_params_f = http_get_vars.find(U("params_f"));
		auto found_alg_params_M = http_get_vars.find(U("params_M"));
		auto found_alg_params_Ninit = http_get_vars.find(U("params_Ninit"));
		auto found_alg_params_IntelMKL = http_get_vars.find(U("params_IntelMKL"));

		if ((found_storage_type == end(http_get_vars))		||
		    (found_storage_path == end(http_get_vars))		||
		    (found_alg_params_C == end(http_get_vars))		||
		    (found_alg_params_Np == end(http_get_vars))		||
		    (found_alg_params_Ns == end(http_get_vars))		||
		    (found_alg_params_f == end(http_get_vars))		||
		    (found_alg_params_f == end(http_get_vars))		||
		    (found_alg_params_Ninit == end(http_get_vars))	||
		    (found_alg_params_IntelMKL == end(http_get_vars))	||
		    (found_alg_params_OpenMP == end(http_get_vars))) {
			auto err = U("Request received with get var \"type\" "
				     "or \"path\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto storage_type = found_storage_type->second;
		auto storage_path = found_storage_path->second;
		
		wstring params_C = found_alg_params_C->second;
		wstring params_Np = found_alg_params_Np->second;
		wstring params_Ns = found_alg_params_Ns->second;
		wstring params_f = found_alg_params_f->second;
		wstring params_M = found_alg_params_M->second;
		wstring params_Ninit = found_alg_params_Ninit->second;

		wstring params_OpenMP = found_alg_params_OpenMP->second;
		wstring params_IntelMKL = found_alg_params_IntelMKL->second;

		std::function<wstring(wstring, const char*, const char*)> replacer =
			[](wstring str, const char* from, const char* to) {
				boost::replace_all(str, from, to);
				return str;
			};


		boost::replace_all(params_C, "%2C", ",");
		boost::replace_all(params_Np, "%2C", ",");
		boost::replace_all(params_Ns, "%2C", ",");
		boost::replace_all(params_f, "%2C", ",");
		boost::replace_all(params_M, "%2C", ",");
		boost::replace_all(params_Ninit, "%2C", ",");
		boost::replace_all(params_OpenMP, "%2C", ",");
		boost::replace_all(params_IntelMKL, "%2C", ",");

		AlgorithmParams algParams;
		algParams.C = (double) std::stod(replacer(
			replacer(params_C, "%2C", ","), ",", "."));
		algParams.Np = (uint32_t) std::stoi(replacer(
			replacer(params_Np, "%2C", ","), ",", "."));
		algParams.Ns = (uint32_t) std::stoi(replacer(
			replacer(params_Ns, "%2C", ","), ",", "."));
		algParams.f = (double) std::stod(replacer(
			replacer(params_f, "%2C", ","), ",", "."));
		algParams.M = (double)std::stod(replacer(
			replacer(params_M, "%2C", ","), ",", "."));
		algParams.Ninit = (uint32_t) std::stoi(replacer(
			replacer(params_Ninit, "%2C", ","), ",", "."));
		algParams.OpenMP = (int)std::stoi(replacer(replacer(
			params_OpenMP, "%2C", ","), ",", "."));
		algParams.IntelMKL = (int)std::stoi(replacer(replacer(
			params_IntelMKL, "%2C", ","), ",", "."));

		cout << "Received algorithm's parameters:" << endl <<
			"C = " << algParams.C << " (" <<
			utility::conversions::to_utf8string(params_C) << ")" << endl <<
			"Np = " << algParams.Np << " (" <<
			utility::conversions::to_utf8string(params_Np) << ")" << endl <<
			"Ns = " << algParams.Ns << " (" <<
			utility::conversions::to_utf8string(params_Ns) << ")" << endl <<
			"f = " << algParams.f << " (" <<
			utility::conversions::to_utf8string(params_f) << ")" << endl <<
			"M = " << algParams.M << " (" <<
			utility::conversions::to_utf8string(params_M) << ")" << endl <<
			"Ninit = " << algParams.Ninit << " (" <<
			utility::conversions::to_utf8string(params_Ninit) << ")" << endl <<
			"OpenMP = " << algParams.OpenMP << " (" <<
			utility::conversions::to_utf8string(params_OpenMP) << ")" << endl <<
			"IntelMKL = " << algParams.IntelMKL << " (" <<
			utility::conversions::to_utf8string(params_IntelMKL) << ")" << endl;

		wcout << U("Received storage: ") << storage_type << endl;
		InitializeCurves(storage_type, storage_path, &algParams);
#ifdef _OPENMP
		this->SetParallelismMode(algParams.OpenMP);
#endif

		auto array = answer.array();
		json::value root;
		size_t numOfCurves = 0;

		if (storage_type == U("File"))
		{
			numOfCurves = GenFile.GetNumberOfCurves();
		}
		else if (storage_type == U("DataBase"))
		{
			numOfCurves = GenDataBase.GetNumberOfCurves();
		}

		for (size_t i = 0; i < numOfCurves; i++)
		{
			web::json::value obj;
			GeneralizationRequestCurve *requested_curve = &Curves[i];
			obj[L"count_points"] = web::json::value::number(X(CurvesMap[i]).size());
			string_t str = utility::conversions::to_utf16string(requested_curve->GetDBCode());
			obj[L"code"] = web::json::value::value(str);
			obj[L"number"] = web::json::value::value(requested_curve->GetDBNumber());

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

		auto found_alg_params_C = http_get_vars.find(U("params_C"));
		auto found_alg_params_Np = http_get_vars.find(U("params_Np"));
		auto found_alg_params_Ns = http_get_vars.find(U("params_Ns"));
		auto found_alg_params_f = http_get_vars.find(U("params_f"));
		auto found_alg_params_M = http_get_vars.find(U("params_M"));
		auto found_alg_params_Ninit = http_get_vars.find(U("params_Ninit"));

		if ((found_alg_params_C == end(http_get_vars))		||
		    (found_alg_params_Np == end(http_get_vars))		||
		    (found_alg_params_Ns == end(http_get_vars))		||
		    (found_alg_params_f == end(http_get_vars))		||
		    (found_alg_params_M == end(http_get_vars))		||
		    (found_alg_params_Ninit == end(http_get_vars))	|| 
		    (found_curve == end(http_get_vars))) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		wstring params_C = found_alg_params_C->second;
		wstring params_Np = found_alg_params_Np->second;
		wstring params_Ns = found_alg_params_Ns->second;
		wstring params_f = found_alg_params_f->second;
		wstring params_M = found_alg_params_M->second;
		wstring params_Ninit = found_alg_params_Ninit->second;

		boost::replace_all(params_C, "%2C", ",");
		boost::replace_all(params_Np, "%2C", ",");
		boost::replace_all(params_Ns, "%2C", ",");
		boost::replace_all(params_f, "%2C", ",");
		boost::replace_all(params_M, "%2C", ",");
		boost::replace_all(params_Ninit, "%2C", ",");

		std::function<wstring(wstring, const char*, const char*)> replacer =
			[](wstring str, const char* from, const char* to) {
			boost::replace_all(str, from, to);
			return str;
		};

		AlgorithmParams algParams;
		algParams.C = (double)std::stod(replacer(
			replacer(params_C, "%2C", ","), ",", "."));
		algParams.Np = (uint32_t)std::stoi(replacer(
			replacer(params_Np, "%2C", ","), ",", "."));
		algParams.Ns = (uint32_t)std::stoi(replacer(
			replacer(params_Ns, "%2C", ","), ",", "."));
		algParams.f = (double)std::stod(replacer(
			replacer(params_f, "%2C", ","), ",", "."));
		algParams.M = (double)std::stod(replacer(
			replacer(params_M, "%2C", ","), ",", "."));
		algParams.Ninit = (uint32_t)std::stoi(replacer(
			replacer(params_Ninit, "%2C", ","), ",", "."));


		auto request_curve = found_curve->second;
		uint32_t requested_curve_number = std::stoi(request_curve);

		wcout << U("Received request SOURCE_CURVE: ") << request_curve << " ("
			<< requested_curve_number << ")" << endl;

		cout << "Received algorithm's parameters:" << endl <<
			"C = " << algParams.C << " (" <<
			utility::conversions::to_utf8string(params_C) << ")" << endl <<
			"Np = " << algParams.Np << " (" <<
			utility::conversions::to_utf8string(params_Np) << ")" << endl <<
			"Ns = " << algParams.Ns << " (" <<
			utility::conversions::to_utf8string(params_Ns) << ")" << endl <<
			"f = " << algParams.f << " (" <<
			utility::conversions::to_utf8string(params_f) << ")" << endl <<
			"M = " << algParams.M << " (" <<
			utility::conversions::to_utf8string(params_M) << ")" << endl <<
			"Ninit = " << algParams.Ninit << " (" <<
			utility::conversions::to_utf8string(params_Ninit) << ")" << endl;

		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];

		requested_curve->SetParamC(algParams.C);
		requested_curve->SetParamNp(algParams.Np);
		requested_curve->SetParamNinit(algParams.Ninit);
		requested_curve->SetParamf(algParams.f);
		requested_curve->SetParamM(algParams.M);
		requested_curve->SetParamNs(algParams.Ns);

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
	else if ((reqObj == ADDUCTION_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		auto found_result = http_get_vars.find(U("result"));

		if (found_curve == end(http_get_vars) ||
		    found_result == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		auto request_result = found_result->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		uint32_t requested_curve_result = std::stoi(request_result);
		wcout << U("Received request ADDUCTION_CURVE(") << request_result << U("): ")
			<< request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];
		if (!requested_curve_result)
			requested_curve->DispatchEvent(Event_t::ADDUCTION);

		auto array = answer.array();
		json::value root;

		uint32_t countOfAdductedPoints;
		curve *adducted_curve = requested_curve->GetAdductedCurve(countOfAdductedPoints);
		root[L"count_points"] = countOfAdductedPoints;
		for (size_t i = 0; i < countOfAdductedPoints; i++)
		{
			web::json::value x;
			web::json::value y;

			x = web::json::value::number(X(*adducted_curve)[i]);
			y = web::json::value::number(Y(*adducted_curve)[i]);

			web::json::value pointXY;
			pointXY[L"X"] = x;
			pointXY[L"Y"] = y;

			array[i] = pointXY;
		}
		root[L"points"] = array;
		root[L"time"] = requested_curve->GetadductionTimer();
		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}
	else if ((reqObj == SEGMENTATION_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		auto found_result = http_get_vars.find(U("result"));

		if (found_curve == end(http_get_vars) ||
		    found_result == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		auto request_result = found_result->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		uint32_t requested_curve_result = std::stoi(request_result);
		wcout << U("Received request SEGMENTATION_CURVE(") << request_result << U("): ")
			<< request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];
		if (!requested_curve_result)
			requested_curve->DispatchEvent(Event_t::SEGMENTATION);

		auto array_segments = answer.array();
		json::value root;

		uint32_t countOfSegm, countOfInitSegm;
		std::vector<uint32_t> *countOfPointsInSegm = NULL;
		std::vector<curve> *segmented_curve = requested_curve->GetSegmentedCurve(countOfSegm, countOfInitSegm,
											 &countOfPointsInSegm);
		root[L"count_segments"] = countOfSegm;
		root[L"count_init_segments"] = countOfInitSegm;
		for (size_t i = 0; i < countOfSegm; i++)
		{
			web::json::value segmObj;
			segmObj[L"count_points"] = (*countOfPointsInSegm)[i]; 
			auto array = segmObj.array();
			for (size_t j = 0; j < (*countOfPointsInSegm)[i]; j++)
			{
				web::json::value x;
				web::json::value y;

				x = web::json::value::number(X((*segmented_curve)[i])[j]);
				y = web::json::value::number(Y((*segmented_curve)[i])[j]);

				web::json::value pointXY;
				pointXY[L"X"] = x;
				pointXY[L"Y"] = y;

				array[j] = pointXY;
			}
			segmObj[L"segment"] = array;
			array_segments[i] = segmObj;
		}
		root[L"segments"] = array_segments;
		root[L"time"] = requested_curve->GetsegmentationTimer();
		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}
	else if ((reqObj == SIMPLIFICATION_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		auto found_result = http_get_vars.find(U("result"));

		if (found_curve == end(http_get_vars) ||
		    found_result == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		auto request_result = found_result->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		uint32_t requested_curve_result = std::stoi(request_result);
		wcout << U("Received request SIMPLIFICATION_CURVE(") << request_result << U("): ")
			<< request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];
		if (!requested_curve_result)
			requested_curve->DispatchEvent(Event_t::SIMPLIFICATION);

		auto array_segments = answer.array();
		json::value root;

		uint32_t countOfSimplSegm;
		uint32_t totalCountOfSimplPoints;
		std::vector<uint32_t> *countOfPointsInSimplSegm = NULL;
		curve** segmented_curve = requested_curve->GetSimplifiedCurve(countOfSimplSegm, totalCountOfSimplPoints,
									      &countOfPointsInSimplSegm);
		root[L"count_segments"] = countOfSimplSegm;
		root[L"total_count_points"] = totalCountOfSimplPoints;
		for (size_t i = 0; i < countOfSimplSegm; i++)
		{
			web::json::value segmObj;
			segmObj[L"count_points"] = (*countOfPointsInSimplSegm)[i];
			auto array = segmObj.array();
			for (size_t j = 0; j < (*countOfPointsInSimplSegm)[i]; j++)
			{
				web::json::value x;
				web::json::value y;

				x = web::json::value::number(X(*(segmented_curve)[i])[j]);
				y = web::json::value::number(Y(*(segmented_curve)[i])[j]);

				web::json::value pointXY;
				pointXY[L"X"] = x;
				pointXY[L"Y"] = y;

				array[j] = pointXY;
			}
			segmObj[L"segment"] = array;
			array_segments[i] = segmObj;
		}
		root[L"segments"] = array_segments;
		root[L"time"] = requested_curve->GetsimplificationTimer();
		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}
	else if ((reqObj == SMOOTHING_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		auto found_result = http_get_vars.find(U("result"));

		if (found_curve == end(http_get_vars) ||
		    found_result == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		auto request_result = found_result->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		uint32_t requested_curve_result = std::stoi(request_result);
		wcout << U("Received request SMOOTHING_CURVE(") << request_result << U("): ")
			<< request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];
		if (!requested_curve_result)
			requested_curve->DispatchEvent(Event_t::SMOOTHING);

		auto array_segments = answer.array();
		json::value root;

		uint32_t countOfSmoothSegm, totalCountOfSmoothPoints;
		std::vector<uint32_t> *countOfPointsInSmoothSegm = NULL;
		curve** segmented_curve = requested_curve->GetSmoothedCurve(countOfSmoothSegm,
			totalCountOfSmoothPoints,
			&countOfPointsInSmoothSegm);
		root[L"count_segments"] = countOfSmoothSegm;
		root[L"total_count_points"] = totalCountOfSmoothPoints;
		for (size_t i = 0; i < countOfSmoothSegm; i++)
		{
			web::json::value segmObj;
			segmObj[L"count_points"] = (*countOfPointsInSmoothSegm)[i];
			auto array = segmObj.array();
			for (size_t j = 0; j < (*countOfPointsInSmoothSegm)[i]; j++)
			{
				web::json::value x;
				web::json::value y;

				x = web::json::value::number(X(*(segmented_curve)[i])[j]);
				y = web::json::value::number(Y(*(segmented_curve)[i])[j]);

				web::json::value pointXY;
				pointXY[L"X"] = x;
				pointXY[L"Y"] = y;

				array[j] = pointXY;
			}
			segmObj[L"segment"] = array;
			array_segments[i] = segmObj;
		}
		root[L"segments"] = array_segments;
		root[L"time"] = requested_curve->GetsmoothingTimer();
		root[L"overall_time"] = requested_curve->GettotalTimer();

		curve *source_curve = requested_curve->GetSouceCurve();
		double_t source_curve_coeff = requested_curve->ComputeSinuosityCoeff(source_curve,
			X(*source_curve).size(), countOfSmoothSegm);

		curve *result_curve = requested_curve->GetResultCurve();
		double_t result_curve_coeff = requested_curve->ComputeSinuosityCoeff(result_curve,
			X(*result_curve).size(), countOfSmoothSegm);
		delete result_curve;

		root[L"sinuosity_coef_source"] = source_curve_coeff;
		root[L"sinuosity_coef_result"] = result_curve_coeff;

		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}
	else if ((reqObj == SAVE_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		int err;

		if (found_curve == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		wcout << U("Received request SAVE_CURVE: ") << request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];

		err = GenDataBase.UpdateDataBaseObject(requested_curve);
		cout << "We are ready to reply" << endl;

		request.reply((err != 0) ? status_codes::InternalError : status_codes::OK);
		return;
	}
	else if ((reqObj == GENERALIZE_CURVE) && (initialized))
	{
		auto found_curve = http_get_vars.find(U("curve_number"));
		int err;

		if (found_curve == end(http_get_vars)) {
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto request_curve = found_curve->second;
		uint32_t requested_curve_number = std::stoi(request_curve);
		wcout << U("Received request GENERALIZE_CURVE: ") << request_curve << " ("
			<< requested_curve_number << ")" << endl;
		GeneralizationRequestCurve *requested_curve = &Curves[requested_curve_number];

		requested_curve->DispatchEvent(Event_t::ADDUCTION);
		wcout << U("Step ADDUCTION done") << endl;
		requested_curve->DispatchEvent(Event_t::SEGMENTATION);
		wcout << U("Step SEGMENTATION done") << endl;
		requested_curve->DispatchEvent(Event_t::SIMPLIFICATION);
		wcout << U("Step SIMPLIFICATION done") << endl;
		requested_curve->DispatchEvent(Event_t::SMOOTHING);
		wcout << U("Step SMOOTHING done") << endl;


		/* Send the resutls only result curve, i.e. Smoothed curve */
		/* If threse are needs, 'result' field should be specified
		 * with the appropriate request */
		auto array_segments = answer.array();
		json::value root;

		uint32_t countOfSmoothSegm, totalCountOfSmoothPoints;
		std::vector<uint32_t> *countOfPointsInSmoothSegm = NULL;
		curve** segmented_curve = requested_curve->GetSmoothedCurve(countOfSmoothSegm,
			totalCountOfSmoothPoints,
			&countOfPointsInSmoothSegm);
		root[L"count_segments"] = countOfSmoothSegm;
		root[L"total_count_points"] = totalCountOfSmoothPoints;
		for (size_t i = 0; i < countOfSmoothSegm; i++)
		{
			web::json::value segmObj;
			segmObj[L"count_points"] = (*countOfPointsInSmoothSegm)[i];
			auto array = segmObj.array();
			for (size_t j = 0; j < (*countOfPointsInSmoothSegm)[i]; j++)
			{
				web::json::value x;
				web::json::value y;

				x = web::json::value::number(X(*(segmented_curve)[i])[j]);
				y = web::json::value::number(Y(*(segmented_curve)[i])[j]);

				web::json::value pointXY;
				pointXY[L"X"] = x;
				pointXY[L"Y"] = y;

				array[j] = pointXY;
			}
			segmObj[L"segment"] = array;
			array_segments[i] = segmObj;
		}
		root[L"segments"] = array_segments;
		root[L"time"] = requested_curve->GetsmoothingTimer();
		root[L"overall_time"] = requested_curve->GettotalTimer();

		curve *source_curve = requested_curve->GetSouceCurve();
		double_t source_curve_coeff = requested_curve->ComputeSinuosityCoeff(source_curve,
			X(*source_curve).size(), countOfSmoothSegm);

		curve *result_curve = requested_curve->GetResultCurve();
		double_t result_curve_coeff = requested_curve->ComputeSinuosityCoeff(result_curve,
			X(*result_curve).size(), countOfSmoothSegm);
		delete result_curve;

		root[L"sinuosity_coef_source"] = source_curve_coeff;
		root[L"sinuosity_coef_result"] = result_curve_coeff;

		cout << "We are ready to reply" << endl;
		request.reply(status_codes::OK, root);
		return;
	}
	else if (reqObj == BENCHMARK)
	{
		auto points_per_seg = http_get_vars.find(U("points_per_seg"));
		auto min_seg_cnt = http_get_vars.find(U("min_seg_cnt"));
		auto max_seg_cnt = http_get_vars.find(U("max_seg_cnt"));
		auto found_alg_params_OpenMP = http_get_vars.find(U("params_OpenMP"));
		auto found_alg_params_C = http_get_vars.find(U("params_C"));
		auto found_alg_params_Np = http_get_vars.find(U("params_Np"));
		auto found_alg_params_Ns = http_get_vars.find(U("params_Ns"));
		auto found_alg_params_f = http_get_vars.find(U("params_f"));
		auto found_alg_params_M = http_get_vars.find(U("params_M"));
		auto found_alg_params_Ninit = http_get_vars.find(U("params_Ninit"));
		auto found_alg_params_IntelMKL = http_get_vars.find(U("params_IntelMKL"));

		if ((found_alg_params_C == end(http_get_vars)) ||
		    (found_alg_params_Np == end(http_get_vars)) ||
		    (found_alg_params_Ns == end(http_get_vars)) ||
		    (found_alg_params_f == end(http_get_vars)) ||
		    (found_alg_params_f == end(http_get_vars)) ||
		    (found_alg_params_Ninit == end(http_get_vars)) ||
		    (found_alg_params_IntelMKL == end(http_get_vars)) ||
		    (found_alg_params_OpenMP == end(http_get_vars))) {
			auto err = U("Request received with get var \"type\" "
				"or \"path\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		wstring params_C = found_alg_params_C->second;
		wstring params_Np = found_alg_params_Np->second;
		wstring params_Ns = found_alg_params_Ns->second;
		wstring params_f = found_alg_params_f->second;
		wstring params_M = found_alg_params_M->second;
		wstring params_Ninit = found_alg_params_Ninit->second;

		wstring params_OpenMP = found_alg_params_OpenMP->second;
		wstring params_IntelMKL = found_alg_params_IntelMKL->second;

		std::function<wstring(wstring, const char*, const char*)> replacer =
			[](wstring str, const char* from, const char* to) {
			boost::replace_all(str, from, to);
			return str;
		};


		boost::replace_all(params_C, "%2C", ",");
		boost::replace_all(params_Np, "%2C", ",");
		boost::replace_all(params_Ns, "%2C", ",");
		boost::replace_all(params_f, "%2C", ",");
		boost::replace_all(params_M, "%2C", ",");
		boost::replace_all(params_Ninit, "%2C", ",");
		boost::replace_all(params_OpenMP, "%2C", ",");
		boost::replace_all(params_IntelMKL, "%2C", ",");

		AlgorithmParams algParams;
		algParams.C = (double)std::stod(replacer(
			replacer(params_C, "%2C", ","), ",", "."));
		algParams.Np = (uint32_t)std::stoi(replacer(
			replacer(params_Np, "%2C", ","), ",", "."));
		algParams.Ns = (uint32_t)std::stoi(replacer(
			replacer(params_Ns, "%2C", ","), ",", "."));
		algParams.f = (double)std::stod(replacer(
			replacer(params_f, "%2C", ","), ",", "."));
		algParams.M = (double)std::stod(replacer(
			replacer(params_M, "%2C", ","), ",", "."));
		algParams.Ninit = (uint32_t)std::stoi(replacer(
			replacer(params_Ninit, "%2C", ","), ",", "."));
		algParams.OpenMP = (int)std::stoi(replacer(replacer(
			params_OpenMP, "%2C", ","), ",", "."));
		algParams.IntelMKL = (int)std::stoi(replacer(replacer(
			params_IntelMKL, "%2C", ","), ",", "."));

		cout << "Received algorithm's parameters:" << endl <<
			"C = " << algParams.C << " (" <<
			utility::conversions::to_utf8string(params_C) << ")" << endl <<
			"Np = " << algParams.Np << " (" <<
			utility::conversions::to_utf8string(params_Np) << ")" << endl <<
			"Ns = " << algParams.Ns << " (" <<
			utility::conversions::to_utf8string(params_Ns) << ")" << endl <<
			"f = " << algParams.f << " (" <<
			utility::conversions::to_utf8string(params_f) << ")" << endl <<
			"M = " << algParams.M << " (" <<
			utility::conversions::to_utf8string(params_M) << ")" << endl <<
			"Ninit = " << algParams.Ninit << " (" <<
			utility::conversions::to_utf8string(params_Ninit) << ")" << endl <<
			"OpenMP = " << algParams.OpenMP << " (" <<
			utility::conversions::to_utf8string(params_OpenMP) << ")" << endl <<
			"IntelMKL = " << algParams.IntelMKL << " (" <<
			utility::conversions::to_utf8string(params_IntelMKL) << ")" << endl;

		int err;

		if ((points_per_seg == end(http_get_vars)) ||
		    (min_seg_cnt == end(http_get_vars)) ||
		    (max_seg_cnt == end(http_get_vars))){
			auto err = U("Request received with get var \"curve_number\" omitted from query.");
			wcout << err << endl;
			/* BAD */
			request.reply(status_codes::BadRequest);
			return;
		}

		auto requested_pps = points_per_seg->second;
		auto requested_min_seg_cnt = min_seg_cnt->second;
		auto requested_max_seg_cnt = max_seg_cnt->second;

		uint32_t pps = std::stoi(requested_pps);
		uint32_t min_seg_count = std::stoi(requested_min_seg_cnt);
		uint32_t max_seg_count = std::stoi(requested_max_seg_cnt);


		wcout << U("Received request BENCHMARK: point per segment = ") << pps <<
			U(" minimum segment count = ") << min_seg_count <<
			U(" maximum segment count = ") << max_seg_count << endl;

		InitializeBenchmark(pps, min_seg_count, max_seg_count, algParams);
		json::value root;
		auto array_timers = answer.array();
		for (uint32_t i = 0; i < max_seg_count - min_seg_count + 1; i++)
		{
			GeneralizationBenchmark *benchmark_curve = &BenchCurves[i];
			benchmark_curve->SetUseOpenMP(algParams.OpenMP);
			benchmark_curve->SetUseMkl(algParams.IntelMKL);
			benchmark_curve->InitForBenchmarks();

			benchmark_curve->RunBenchmarkOnSegmentedCurve();
			web::json::value itemTimer;

			itemTimer[L"simpl_time"] = benchmark_curve->GetsimplificationTimer();
			itemTimer[L"smooth_time"] = benchmark_curve->GetsmoothingTimer();

			array_timers[i] = itemTimer;

			benchmark_curve->~GeneralizationBenchmark();
		}
		root[L"timers"] = array_timers;


		FinalizeBenchmark();
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
				auto err = U("Request received with get var \"curve\" omitted from query.");
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
		[&](json::value & jvalue, json::value & answer)
		{
			auto http_get_vars = uri::split_query(request.request_uri().query());
			wcout << "QUERY : " << endl;
			wcout << request.request_uri().query() << endl;
			auto found_name = http_get_vars.find(U("/curve"));

			if (found_name == end(http_get_vars)) {
				auto err = U("Request received with get var \"curve\" omitted from query.");
				wcout << err << endl;
				/* BAD */
				return;
			}

			auto request_name = found_name->second;
			wcout << U("Received request: ") << request_name << endl;
		});
}

void GeneralizationServer::handle_put(http_request request)
{
	std::cout << "\nhandle PUT\n" << std::endl;

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
	});
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
