#include "engine.h"

#include <strstream>
#include <sstream>
#include <cstdio>

#include "../JASP-Common/lib_json/json.h"
#include "../JASP-Common/analysisloader.h"
#include "../JASP-Common/processinfo.h"
#include "../JASP-Common/datasetloader.h"
#include "../JASP-Common/tempfiles.h"
#include "../JASP-Common/utils.h"

#include "rbridge.h"



#ifdef __WIN32__

#undef Realloc
#undef Free

#endif

using namespace std;
using namespace boost::interprocess;
using namespace boost::posix_time;

Engine::Engine()
{
	_dataSet = NULL;
	_channel = NULL;
	_slaveNo = 0;

	_status = empty;

	rbridge_init();
	tempfiles_attach(ProcessInfo::parentPID());

	DataSet *dataSet = DataSetLoader::getDataSet();
	rbridge_setDataSet(dataSet);

	rbridge_setFileNameSource(boost::bind(&Engine::provideTempFileName, this, _1, _2, _3));
	rbridge_setStateFileSource(boost::bind(&Engine::provideStateFileName, this));
}

void Engine::setSlaveNo(int no)
{
	_slaveNo = no;
}

void Engine::runAnalysis()
{
	if (_status == empty || _status == aborted)
		return;

	string perform;

	if (_status == toInit)
	{
		perform = "init";
		_status = initing;
	}
	else
	{
		perform = "run";
		_status = running;
	}

	vector<string> tempFilesFromLastTime = tempfiles_retrieveList(_analysisId);

	RCallback callback = boost::bind(&Engine::callback, this, _1);

	_analysisResultsString = rbridge_run(_analysisName, _analysisOptions, perform, _ppi, callback);

	if (_status == toInit || _status == aborted || _status == error)
	{
		// analysis was aborted, and we shouldn't send the results
	}
	else if (_status == changed && _analysisResultsString == "null")
	{
		// analysis was changed, and the analysis could not incorporate the changes
		// so the analysis returned null, and now needs to be re-run

		_status = toInit;
	}
	else
	{
		Json::Reader parser;
		parser.parse(_analysisResultsString, _analysisResults, false);

		_status = _status == initing ? inited : complete;
		sendResults();
		_status = empty;

		vector<string> filesToKeep;

		if (_analysisResults.isObject())
		{
			Json::Value filesToKeepValue = _analysisResults.get("keep", Json::nullValue);

			if (filesToKeepValue.isArray())
			{
				for (int i = 0; i < filesToKeepValue.size(); i++)
				{
					Json::Value fileToKeepValue = filesToKeepValue.get(i, Json::nullValue);
					if ( ! fileToKeepValue.isString())
						continue;

					filesToKeep.push_back(fileToKeepValue.asString());
				}
			}
			else if (filesToKeepValue.isString())
			{
				filesToKeep.push_back(filesToKeepValue.asString());
			}
		}

		Utils::remove(tempFilesFromLastTime, filesToKeep);

		tempfiles_deleteList(tempFilesFromLastTime);
	}
}

void Engine::run()
{
	stringstream ss;
	ss << "JASP-IPC-" << ProcessInfo::parentPID();
	string memoryName = ss.str();

	_channel = new IPCChannel(memoryName, _slaveNo, true);

	do
	{
		receiveMessages(100);
		if ( ! ProcessInfo::isParentRunning())
			break;
		runAnalysis();

	}
	while(1);

	shared_memory_object::remove(memoryName.c_str());
}

bool Engine::receiveMessages(int timeout)
{
	string data;

	if (_channel->receive(data, timeout))
	{
		Json::Value jsonRequest;
		Json::Reader r;
		r.parse(data, jsonRequest, false);

		int analysisId = jsonRequest.get("id", -1).asInt();
		string perform = jsonRequest.get("perform", "run").asString();

		if (analysisId == _analysisId && _status == running)
		{
			// if the current running analysis has changed

			if (perform == "init")
				_status = changed;
			else if (perform == "stop")
				_status = stopped;
			else
				_status = aborted;
		}
		else
		{
			// the new analysis should be init or run (existing analyses will be aborted)

			_analysisId = analysisId;

			if (perform == "init")
				_status = toInit;
			else if (perform == "run")
				_status = toRun;
			else
				_status = error;
		}

		if (_status == toInit || _status == toRun || _status == changed)
		{
			_analysisName = jsonRequest.get("name", Json::nullValue).asString();
			_analysisOptions = jsonRequest.get("options", Json::nullValue).toStyledString();

			Json::Value settings = jsonRequest.get("settings", Json::nullValue);
			if (settings.isObject())
			{
				Json::Value ppi = settings.get("ppi", Json::nullValue);
				_ppi = ppi.isInt() ? ppi.asInt() : 96;
			}
			else
			{
				_ppi = 96;
			}
		}

		return true;
	}

	return false;
}

void Engine::sendResults()
{
	Json::Value response = Json::Value(Json::objectValue);

	response["id"] = _analysisId;
	response["name"] = _analysisName;

	Json::Value resultsStatus = Json::nullValue;

	if (_analysisResults.isObject())
		resultsStatus = _analysisResults.get("status", Json::nullValue);

	if (resultsStatus != Json::nullValue)
	{
		response["results"] = _analysisResults.get("results", Json::nullValue);
		response["status"]  = resultsStatus.asString();
	}
	else
	{
		string status;

		switch (_status)
		{
		case inited:
			status = "inited";
			break;
		case running:
		case changed:
			status = "running";
			break;
		case complete:
			status = "complete";
			break;
		case stopped:
			status = "stopped";
			break;
		default:
			status = "error";
			break;
		}

		response["results"] = _analysisResults;
		response["status"] = status;
	}

	string message = response.toStyledString();

	_channel->send(message);
}

string Engine::callback(const string &results)
{
	receiveMessages();

	if (_status == aborted || _status == toInit || _status == toRun)
		return "{ \"status\" : \"aborted\" }"; // abort

	if (results != "null")
	{
		_analysisResultsString = results;

		Json::Reader parser;
		parser.parse(_analysisResultsString, _analysisResults, false);

		sendResults();
	}

	if (_status == changed)
		return "{ \"status\" : \"changed\", \"options\" : " + _analysisOptions + " }";
	else if (_status == stopped)
		return "{ \"status\" : \"stopped\" }";
	else if (_status == aborted)
		return "{ \"status\" : \"aborted\" }";

	return "{ \"status\" : \"ok\" }";
}

string Engine::provideStateFileName()
{
	return tempfiles_createSpecific("state", _analysisId);
}

void Engine::provideTempFileName(const string &extension, string &root, string &relativePath)
{	
	tempfiles_create(extension, _analysisId, root, relativePath);
}

