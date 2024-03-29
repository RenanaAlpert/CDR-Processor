#include "QueriesKinds.h"
#include "HendlerFunction.h"

namespace experis
{

std::string Query::Response(std::vector<std::string>& query, const DataBase_mt& a_db) const
{
	return query.at(0) + " is illegal query\n";
}

std::string Msisdn::Response(std::vector<std::string>& a_query, const DataBase_mt& a_db) const
{
	std::vector<std::pair<std::string, DATA>> data{};
	MSISDN msisdn = detail::IsNumber(a_query.at(1)) ? std::stoull(a_query.at(1)) : 0ULL;
	if(!a_db.ContainsMsisdn(msisdn))
	{
		return "subscirbe " + std::to_string(msisdn) + " is not exiest in system\n";
	}
	data.push_back({"\"msisdn\"" , msisdn});
	data.push_back({"\"voice-out\"" , a_db.GetMsisdn(msisdn, MOC)});
	data.push_back({"\"voice-in\"" , a_db.GetMsisdn(msisdn, MTC)});
	data.push_back({"\"data-out\"" , a_db.GetMsisdn(msisdn, DO)});
	data.push_back({"\"data-in\"" , a_db.GetMsisdn(msisdn, DI)});
	data.push_back({"\"sms-out\"" , a_db.GetMsisdn(msisdn, SMS_MO)});
	data.push_back({"\"sms-in\"" , a_db.GetMsisdn(msisdn, SMS_MT)});

	return JsonFormat(data);
}

std::string Operator::Response(std::vector<std::string>& a_query, const DataBase_mt& a_db) const
{
	std::vector<std::pair<std::string, DATA>> data{};
	IMSI imsi = detail::IsNumber(a_query.at(1)) ? std::stoi(a_query.at(1)) : 0;
	if(!a_db.ContainsImsi(imsi))
	{
		return "subscirbe " + std::to_string(imsi) + " is not exiest in system\n";
	}
	data.push_back({"\"voice-out\"" , a_db.GetOperator(imsi, MOC)});
	data.push_back({"\"voice-in\"" , a_db.GetOperator(imsi, MTC)});
	data.push_back({"\"sms-out\"" ,a_db.GetOperator(imsi, SMS_MO)});
	data.push_back({"\"sms-in\"" , a_db.GetOperator(imsi, SMS_MT)});

	return JsonFormat(data);
}

std::string ReturnConaction(std::pair<DATA, DATA> a_conection)
{
	std::vector<std::pair<std::string, DATA>> data{};
	data.push_back({"\"voice-call\"" , a_conection.first});
	data.push_back({"\"sms-volume\"" , a_conection.second});
	return JsonFormat(data);
}

std::string ReturnPath(const std::list<MSISDN>& a_path)
{
	std::string path{};
	for(MSISDN msisdn : a_path)
	{
		path += "->" + msisdn;
	}
	path += "\n";
	return path;
}

std::string Link::Response(std::vector<std::string>& a_query, const DataBase_mt& a_db) const
{
	MSISDN msisdn1 = detail::IsNumber(a_query.at(1)) ? std::stoull(a_query.at(1)) : 0ULL;
	if(!a_db.ContainsMsisdn(msisdn1))
	{
		return "subscirbe " + std::to_string(msisdn1) + " is not exiest in system\n";
	}
	MSISDN msisdn2 = detail::IsNumber(a_query.at(2)) ? std::stoull(a_query.at(2)) : 0ULL;
	if(!a_db.ContainsLink(msisdn1, msisdn2))
	{
		return "subscirbe " + std::to_string(msisdn2) + " is not exiest in system\n";
	}
	std::pair<DATA, DATA> conection{};
	std::list<MSISDN> path = a_db.GetPath(msisdn1, msisdn2, conection);
	if(path.empty() && conection.first == 0 && conection.second == 0)
	{
		return "there are not path between " + std::to_string(msisdn1) + " to " + std::to_string(msisdn2) + "\n";
	}
	return path.empty() ? ReturnConaction(conection) : ReturnPath(path);
}

std::string Start::Response(std::vector<std::string>& a_query, const DataBase_mt& a_db) const
{
	return "wellcom\n";
}

bool detail::IsNumber(const std::string& a_text)
{
	for(char c : a_text)
	{
		if(!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

} // experis